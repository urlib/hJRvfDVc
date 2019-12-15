import sys
import bisect
import types

from _pydev_imps._pydev_saved_modules import threading
from _pydevd_bundle import pydevd_utils, pydevd_source_mapping
from _pydevd_bundle.pydevd_additional_thread_info import set_additional_thread_info
from _pydevd_bundle.pydevd_comm import (InternalGetThreadStack, internal_get_completions,
    pydevd_find_thread_by_id, InternalSetNextStatementThread, internal_reload_code,
    InternalGetVariable, InternalGetArray, InternalLoadFullValue,
    internal_get_description, internal_get_frame, internal_evaluate_expression, InternalConsoleExec,
    internal_get_variable_json, internal_change_variable, internal_change_variable_json,
    internal_evaluate_expression_json, internal_set_expression_json, internal_get_exception_details_json,
    internal_step_in_thread, internal_run_thread, run_as_pydevd_daemon_thread)
from _pydevd_bundle.pydevd_comm_constants import (CMD_THREAD_SUSPEND, file_system_encoding,
    CMD_STEP_INTO_MY_CODE, CMD_STOP_ON_START)
from _pydevd_bundle.pydevd_constants import (get_current_thread_id, set_protocol, get_protocol,
    HTTP_JSON_PROTOCOL, JSON_PROTOCOL, IS_PY3K, DebugInfoHolder, dict_keys, dict_items, IS_WINDOWS)
from _pydevd_bundle.pydevd_net_command_factory_json import NetCommandFactoryJson
from _pydevd_bundle.pydevd_net_command_factory_xml import NetCommandFactory
import pydevd_file_utils
from _pydev_bundle import pydev_log
from _pydevd_bundle.pydevd_breakpoints import LineBreakpoint
from pydevd_tracing import get_exception_traceback_str
import os
import subprocess
import ctypes

try:
    import dis
except ImportError:

    def _get_code_lines(code):
        raise NotImplementedError

else:

    def _get_code_lines(code):
        if not isinstance(code, types.CodeType):
            path = code
            with open(path) as f:
                src = f.read()
            code = compile(src, path, 'exec', 0, dont_inherit=True)
            return _get_code_lines(code)

        def iterate():
            # First, get all line starts for this code object. This does not include
            # bodies of nested class and function definitions, as they have their
            # own objects.
            for _, lineno in dis.findlinestarts(code):
                yield lineno

            # For nested class and function definitions, their respective code objects
            # are constants referenced by this object.
            for const in code.co_consts:
                if isinstance(const, types.CodeType) and const.co_filename == code.co_filename:
                    for lineno in _get_code_lines(const):
                        yield lineno

        return iterate()


class PyDevdAPI(object):

    def run(self, py_db):
        py_db.ready_to_run = True

    def notify_initialize(self, py_db):
        py_db.on_initialize()

    def notify_configuration_done(self, py_db):
        py_db.on_configuration_done()

    def notify_disconnect(self, py_db):
        py_db.on_disconnect()

    def set_protocol(self, py_db, seq, protocol):
        set_protocol(protocol.strip())
        if get_protocol() in (HTTP_JSON_PROTOCOL, JSON_PROTOCOL):
            cmd_factory_class = NetCommandFactoryJson
        else:
            cmd_factory_class = NetCommandFactory

        if not isinstance(py_db.cmd_factory, cmd_factory_class):
            py_db.cmd_factory = cmd_factory_class()

        return py_db.cmd_factory.make_protocol_set_message(seq)

    def set_ide_os_and_breakpoints_by(self, py_db, seq, ide_os, breakpoints_by):
        '''
        :param ide_os: 'WINDOWS' or 'UNIX'
        :param breakpoints_by: 'ID' or 'LINE'
        '''
        if breakpoints_by == 'ID':
            py_db._set_breakpoints_with_id = True
        else:
            py_db._set_breakpoints_with_id = False

        self.set_ide_os(ide_os)

        return py_db.cmd_factory.make_version_message(seq)

    def set_ide_os(self, ide_os):
        '''
        :param ide_os: 'WINDOWS' or 'UNIX'
        '''
        pydevd_file_utils.set_ide_os(ide_os)

    def send_error_message(self, py_db, msg):
        sys.stderr.write('pydevd: %s\n' % (msg,))

    def set_show_return_values(self, py_db, show_return_values):
        if show_return_values:
            py_db.show_return_values = True
        else:
            if py_db.show_return_values:
                # We should remove saved return values
                py_db.remove_return_values_flag = True
            py_db.show_return_values = False
        pydev_log.debug("Show return values: %s", py_db.show_return_values)

    def list_threads(self, py_db, seq):
        # Response is the command with the list of threads to be added to the writer thread.
        return py_db.cmd_factory.make_list_threads_message(py_db, seq)

    def request_suspend_thread(self, py_db, thread_id='*'):
        # Yes, thread suspend is done at this point, not through an internal command.
        threads = []
        suspend_all = thread_id.strip() == '*'
        if suspend_all:
            threads = pydevd_utils.get_non_pydevd_threads()

        elif thread_id.startswith('__frame__:'):
            sys.stderr.write("Can't suspend tasklet: %s\n" % (thread_id,))

        else:
            threads = [pydevd_find_thread_by_id(thread_id)]

        for t in threads:
            if t is None:
                continue
            py_db.set_suspend(
                t,
                CMD_THREAD_SUSPEND,
                suspend_other_threads=suspend_all,
                is_pause=True,
            )
            # Break here (even if it's suspend all) as py_db.set_suspend will
            # take care of suspending other threads.
            break

    def set_enable_thread_notifications(self, py_db, enable):
        '''
        When disabled, no thread notifications (for creation/removal) will be
        issued until it's re-enabled.

        Note that when it's re-enabled, a creation notification will be sent for
        all existing threads even if it was previously sent (this is meant to
        be used on disconnect/reconnect).
        '''
        py_db.set_enable_thread_notifications(enable)

    def request_disconnect(self, py_db, resume_threads):
        self.set_enable_thread_notifications(py_db, False)
        self.remove_all_breakpoints(py_db, filename='*')
        self.remove_all_exception_breakpoints(py_db)
        self.notify_disconnect(py_db)

        if resume_threads:
            self.request_resume_thread(thread_id='*')

    def request_resume_thread(self, thread_id):
        threads = []
        if thread_id == '*':
            threads = pydevd_utils.get_non_pydevd_threads()

        elif thread_id.startswith('__frame__:'):
            sys.stderr.write("Can't make tasklet run: %s\n" % (thread_id,))

        else:
            threads = [pydevd_find_thread_by_id(thread_id)]

        for t in threads:
            if t is None:
                continue

            internal_run_thread(t, set_additional_thread_info=set_additional_thread_info)

    def request_completions(self, py_db, seq, thread_id, frame_id, act_tok, line=-1, column=-1):
        py_db.post_method_as_internal_command(
            thread_id, internal_get_completions, seq, thread_id, frame_id, act_tok, line=line, column=column)

    def request_stack(self, py_db, seq, thread_id, fmt=None, timeout=.5, start_frame=0, levels=0):
        # If it's already suspended, get it right away.
        internal_get_thread_stack = InternalGetThreadStack(
            seq, thread_id, py_db, set_additional_thread_info, fmt=fmt, timeout=timeout, start_frame=start_frame, levels=levels)
        if internal_get_thread_stack.can_be_executed_by(get_current_thread_id(threading.current_thread())):
            internal_get_thread_stack.do_it(py_db)
        else:
            py_db.post_internal_command(internal_get_thread_stack, '*')

    def request_exception_info_json(self, py_db, request, thread_id, max_frames):
        py_db.post_method_as_internal_command(
            thread_id,
            internal_get_exception_details_json,
            request,
            thread_id,
            max_frames,
            set_additional_thread_info=set_additional_thread_info,
            iter_visible_frames_info=py_db.cmd_factory._iter_visible_frames_info,
        )

    def request_step(self, py_db, thread_id, step_cmd_id):
        t = pydevd_find_thread_by_id(thread_id)
        if t:
            py_db.post_method_as_internal_command(
                thread_id,
                internal_step_in_thread,
                thread_id,
                step_cmd_id,
                set_additional_thread_info=set_additional_thread_info,
            )
        elif thread_id.startswith('__frame__:'):
            sys.stderr.write("Can't make tasklet step command: %s\n" % (thread_id,))

    def request_set_next(self, py_db, seq, thread_id, set_next_cmd_id, line, func_name):
        t = pydevd_find_thread_by_id(thread_id)
        if t:
            int_cmd = InternalSetNextStatementThread(thread_id, set_next_cmd_id, line, func_name, seq=seq)
            py_db.post_internal_command(int_cmd, thread_id)
        elif thread_id.startswith('__frame__:'):
            sys.stderr.write("Can't set next statement in tasklet: %s\n" % (thread_id,))

    def request_reload_code(self, py_db, seq, module_name):
        thread_id = '*'  # Any thread
        # Note: not going for the main thread because in this case it'd only do the load
        # when we stopped on a breakpoint.
        py_db.post_method_as_internal_command(
            thread_id, internal_reload_code, seq, module_name)

    def request_change_variable(self, py_db, seq, thread_id, frame_id, scope, attr, value):
        '''
        :param scope: 'FRAME' or 'GLOBAL'
        '''
        py_db.post_method_as_internal_command(
            thread_id, internal_change_variable, seq, thread_id, frame_id, scope, attr, value)

    def request_get_variable(self, py_db, seq, thread_id, frame_id, scope, attrs):
        '''
        :param scope: 'FRAME' or 'GLOBAL'
        '''
        int_cmd = InternalGetVariable(seq, thread_id, frame_id, scope, attrs)
        py_db.post_internal_command(int_cmd, thread_id)

    def request_get_array(self, py_db, seq, roffset, coffset, rows, cols, fmt, thread_id, frame_id, scope, attrs):
        int_cmd = InternalGetArray(seq, roffset, coffset, rows, cols, fmt, thread_id, frame_id, scope, attrs)
        py_db.post_internal_command(int_cmd, thread_id)

    def request_load_full_value(self, py_db, seq, thread_id, frame_id, vars):
        int_cmd = InternalLoadFullValue(seq, thread_id, frame_id, vars)
        py_db.post_internal_command(int_cmd, thread_id)

    def request_get_description(self, py_db, seq, thread_id, frame_id, expression):
        py_db.post_method_as_internal_command(
            thread_id, internal_get_description, seq, thread_id, frame_id, expression)

    def request_get_frame(self, py_db, seq, thread_id, frame_id):
        py_db.post_method_as_internal_command(
            thread_id, internal_get_frame, seq, thread_id, frame_id)

    def to_str(self, s):
        '''
        In py2 converts a unicode to str (bytes) using utf-8.
        -- in py3 raises an error if it's not str already.
        '''
        if s.__class__ != str:
            if not IS_PY3K:
                s = s.encode('utf-8')
            else:
                raise AssertionError('Expected to have str on Python 3. Found: %s (%s)' % (s, s.__class__))
        return s

    def filename_to_str(self, filename):
        '''
        In py2 converts a unicode to str (bytes) using the file system encoding.
        -- in py3 raises an error if it's not str already.
        '''
        if filename.__class__ != str:
            if not IS_PY3K:
                filename = filename.encode(file_system_encoding)
            else:
                raise AssertionError('Expected to have str on Python 3. Found: %s (%s)' % (filename, filename.__class__))
        return filename

    def filename_to_server(self, filename):
        filename = self.filename_to_str(filename)
        return pydevd_file_utils.norm_file_to_server(filename)

    class _DummyFrame(object):
        '''
        Dummy frame to be used with PyDB.apply_files_filter (as we don't really have the
        related frame as breakpoints are added before execution).
        '''

        class _DummyCode(object):

            def __init__(self, filename):
                self.co_firstlineno = 1
                self.co_filename = filename
                self.co_name = 'invalid func name '

        def __init__(self, filename):
            self.f_code = self._DummyCode(filename)
            self.f_globals = {}

    ADD_BREAKPOINT_NO_ERROR = 0
    ADD_BREAKPOINT_FILE_NOT_FOUND = 1
    ADD_BREAKPOINT_FILE_EXCLUDED_BY_FILTERS = 2

    class _AddBreakpointResult(object):

        # :see: ADD_BREAKPOINT_NO_ERROR = 0
        # :see: ADD_BREAKPOINT_FILE_NOT_FOUND = 1
        # :see: ADD_BREAKPOINT_FILE_EXCLUDED_BY_FILTERS = 2

        __slots__ = ['error_code', 'translated_filename', 'translated_line']

        def __init__(self, translated_filename, translated_line):
            self.error_code = PyDevdAPI.ADD_BREAKPOINT_NO_ERROR
            self.translated_filename = translated_filename
            self.translated_line = translated_line

    def add_breakpoint(
            self, py_db, filename, breakpoint_type, breakpoint_id, line, condition, func_name,
            expression, suspend_policy, hit_condition, is_logpoint, adjust_line=False):
        '''
        :param str filename:
            Note: must be sent as it was received in the protocol. It may be translated in this
            function and its final value will be available in the returned _AddBreakpointResult.

        :param str breakpoint_type:
            One of: 'python-line', 'django-line', 'jinja2-line'.

        :param int breakpoint_id:

        :param int line:
            Note: it's possible that a new line was actually used. If that's the case its
            final value will be available in the returned _AddBreakpointResult.

        :param condition:
            Either None or the condition to activate the breakpoint.

        :param str func_name:
            If "None" (str), may hit in any context.
            Empty string will hit only top level.
            Any other value must match the scope of the method to be matched.

        :param str expression:
            None or the expression to be evaluated.

        :param suspend_policy:
            Either "NONE" (to suspend only the current thread when the breakpoint is hit) or
            "ALL" (to suspend all threads when a breakpoint is hit).

        :param str hit_condition:
            An expression where `@HIT@` will be replaced by the number of hits.
            i.e.: `@HIT@ == x` or `@HIT@ >= x`

        :param bool is_logpoint:
            If True and an expression is passed, pydevd will create an io message command with the
            result of the evaluation.

        :return _AddBreakpointResult:
        '''
        assert filename.__class__ == str, 'Expected str, found: %s' % (filename.__class__,)  # i.e.: bytes on py2 and str on py3

        original_filename = filename
        pydev_log.debug('Request for breakpoint in: %s line: %s', original_filename, line)
        # Parameters to reapply breakpoint.
        api_add_breakpoint_params = (filename, breakpoint_type, breakpoint_id, line, condition, func_name,
            expression, suspend_policy, hit_condition, is_logpoint)

        filename = self.filename_to_server(filename)  # Apply user path mapping.
        func_name = self.to_str(func_name)

        assert filename.__class__ == str  # i.e.: bytes on py2 and str on py3
        assert func_name.__class__ == str  # i.e.: bytes on py2 and str on py3

        # Apply source mapping (i.e.: ipython).
        new_filename, new_line, multi_mapping_applied = py_db.source_mapping.map_to_server(filename, line)

        py_db.api_received_breakpoints[(original_filename, breakpoint_id)] = (new_filename, api_add_breakpoint_params)

        pydev_log.debug('Breakpoint (after path/source mapping) in: %s line: %s', new_filename, new_line)

        if multi_mapping_applied:
            # Note that source mapping is internal and does not change the resulting filename nor line
            # (we want the outside world to see the line in the original file and not in the ipython
            # cell, otherwise the editor wouldn't be correct as the returned line is the line to
            # which the breakpoint will be moved in the editor).
            result = self._AddBreakpointResult(filename, line)
            filename = new_filename
            line = new_line

        else:
            if adjust_line and not filename.startswith('<'):
                # Validate breakpoints and adjust their positions.
                try:
                    lines = sorted(_get_code_lines(filename))
                except Exception:
                    pass
                else:
                    if line not in lines:
                        # Adjust to the first preceding valid line.
                        idx = bisect.bisect_left(lines, line)
                        if idx > 0:
                            line = lines[idx - 1]

            result = self._AddBreakpointResult(filename, line)

        if not filename.startswith('<'):
            # Note: if a mapping pointed to a file starting with '<', don't validate.

            if not pydevd_file_utils.exists(filename):
                result.error_code = self.ADD_BREAKPOINT_FILE_NOT_FOUND
                return result

            if (
                    py_db.is_files_filter_enabled and
                    not py_db.get_require_module_for_filters() and
                    py_db.apply_files_filter(self._DummyFrame(filename), filename, False)
                ):
                # Note that if `get_require_module_for_filters()` returns False, we don't do this check.
                # This is because we don't have the module name given a file at this point (in
                # runtime it's gotten from the frame.f_globals).
                # An option could be calculate it based on the filename and current sys.path,
                # but on some occasions that may be wrong (for instance with `__main__` or if
                # the user dynamically changes the PYTHONPATH).

                # Note: depending on the use-case, filters may be changed, so, keep on going and add the
                # breakpoint even with the error code.
                result.error_code = self.ADD_BREAKPOINT_FILE_EXCLUDED_BY_FILTERS

        if breakpoint_type == 'python-line':
            added_breakpoint = LineBreakpoint(line, condition, func_name, expression, suspend_policy, hit_condition=hit_condition, is_logpoint=is_logpoint)
            breakpoints = py_db.breakpoints
            file_to_id_to_breakpoint = py_db.file_to_id_to_line_breakpoint
            supported_type = True

        else:
            add_plugin_breakpoint_result = None
            plugin = py_db.get_plugin_lazy_init()
            if plugin is not None:
                add_plugin_breakpoint_result = plugin.add_breakpoint('add_line_breakpoint', py_db, breakpoint_type, filename, line, condition, expression, func_name, hit_condition=hit_condition, is_logpoint=is_logpoint)
            if add_plugin_breakpoint_result is not None:
                supported_type = True
                added_breakpoint, breakpoints = add_plugin_breakpoint_result
                file_to_id_to_breakpoint = py_db.file_to_id_to_plugin_breakpoint
            else:
                supported_type = False

        if not supported_type:
            raise NameError(breakpoint_type)

        if DebugInfoHolder.DEBUG_TRACE_BREAKPOINTS > 0:
            pydev_log.debug('Added breakpoint:%s - line:%s - func_name:%s\n', filename, line, func_name)

        if filename in file_to_id_to_breakpoint:
            id_to_pybreakpoint = file_to_id_to_breakpoint[filename]
        else:
            id_to_pybreakpoint = file_to_id_to_breakpoint[filename] = {}

        id_to_pybreakpoint[breakpoint_id] = added_breakpoint
        py_db.consolidate_breakpoints(filename, id_to_pybreakpoint, breakpoints)
        if py_db.plugin is not None:
            py_db.has_plugin_line_breaks = py_db.plugin.has_line_breaks()

        py_db.on_breakpoints_changed()
        return result

    def reapply_breakpoints(self, py_db):
        '''
        Reapplies all the received breakpoints as they were received by the API (so, new
        translations are applied).
        '''
        items = dict_items(py_db.api_received_breakpoints)  # Create a copy with items to reapply.
        self.remove_all_breakpoints(py_db, '*')
        for _key, val in items:
            _new_filename, api_add_breakpoint_params = val
            self.add_breakpoint(py_db, *api_add_breakpoint_params)

    def remove_all_breakpoints(self, py_db, filename):
        '''
        Removes all the breakpoints from a given file or from all files if filename == '*'.

        :param str filename:
            Note: must be sent as it was received in the protocol. It may be translated in this
            function.
        '''
        assert filename.__class__ == str  # i.e.: bytes on py2 and str on py3
        changed = False
        lst = [
            py_db.file_to_id_to_line_breakpoint,
            py_db.file_to_id_to_plugin_breakpoint,
            py_db.breakpoints
        ]
        if hasattr(py_db, 'django_breakpoints'):
            lst.append(py_db.django_breakpoints)

        if hasattr(py_db, 'jinja2_breakpoints'):
            lst.append(py_db.jinja2_breakpoints)

        if filename == '*':
            py_db.api_received_breakpoints.clear()

            for file_to_id_to_breakpoint in lst:
                if file_to_id_to_breakpoint:
                    file_to_id_to_breakpoint.clear()
                    changed = True

        else:
            items = dict_items(py_db.api_received_breakpoints)  # Create a copy to remove items.
            translated_filenames = []
            for key, val in items:
                key_filename, _breakpoint_id = key
                if key_filename == filename:
                    new_filename, _api_add_breakpoint_params = val
                    # Note: there can be actually 1:N mappings due to source mapping (i.e.: ipython).
                    translated_filenames.append(new_filename)
                    del py_db.api_received_breakpoints[key]

            for filename in translated_filenames:
                for file_to_id_to_breakpoint in lst:
                    if filename in file_to_id_to_breakpoint:
                        del file_to_id_to_breakpoint[filename]
                        changed = True

        if changed:
            py_db.on_breakpoints_changed(removed=True)

    def remove_breakpoint(self, py_db, filename, breakpoint_type, breakpoint_id):
        '''
        :param str filename:
            Note: must be sent as it was received in the protocol. It may be translated in this
            function.

        :param str breakpoint_type:
            One of: 'python-line', 'django-line', 'jinja2-line'.

        :param int breakpoint_id:
        '''
        for key, val in dict_items(py_db.api_received_breakpoints):
            original_filename, existing_breakpoint_id = key
            _new_filename, _api_add_breakpoint_params = val
            if filename == original_filename and existing_breakpoint_id == breakpoint_id:
                del py_db.api_received_breakpoints[key]
                break
        else:
            pydev_log.info(
                'Did not find breakpoint to remove: %s (breakpoint id: %s)', filename, breakpoint_id)

        file_to_id_to_breakpoint = None
        filename = self.filename_to_server(filename)

        if breakpoint_type == 'python-line':
            breakpoints = py_db.breakpoints
            file_to_id_to_breakpoint = py_db.file_to_id_to_line_breakpoint

        elif py_db.plugin is not None:
            result = py_db.plugin.get_breakpoints(py_db, breakpoint_type)
            if result is not None:
                file_to_id_to_breakpoint = py_db.file_to_id_to_plugin_breakpoint
                breakpoints = result

        if file_to_id_to_breakpoint is None:
            pydev_log.critical('Error removing breakpoint. Cannot handle breakpoint of type %s', breakpoint_type)

        else:
            try:
                id_to_pybreakpoint = file_to_id_to_breakpoint.get(filename, {})
                if DebugInfoHolder.DEBUG_TRACE_BREAKPOINTS > 0:
                    existing = id_to_pybreakpoint[breakpoint_id]
                    pydev_log.info('Removed breakpoint:%s - line:%s - func_name:%s (id: %s)\n' % (
                        filename, existing.line, existing.func_name.encode('utf-8'), breakpoint_id))

                del id_to_pybreakpoint[breakpoint_id]
                py_db.consolidate_breakpoints(filename, id_to_pybreakpoint, breakpoints)
                if py_db.plugin is not None:
                    py_db.has_plugin_line_breaks = py_db.plugin.has_line_breaks()

            except KeyError:
                pydev_log.info("Error removing breakpoint: Breakpoint id not found: %s id: %s. Available ids: %s\n",
                    filename, breakpoint_id, dict_keys(id_to_pybreakpoint))

        py_db.on_breakpoints_changed(removed=True)

    def request_exec_or_evaluate(
            self, py_db, seq, thread_id, frame_id, expression, is_exec, trim_if_too_big, attr_to_set_result):
        py_db.post_method_as_internal_command(
            thread_id, internal_evaluate_expression,
            seq, thread_id, frame_id, expression, is_exec, trim_if_too_big, attr_to_set_result)

    def request_exec_or_evaluate_json(
            self, py_db, request, thread_id):
        py_db.post_method_as_internal_command(
            thread_id, internal_evaluate_expression_json, request, thread_id)

    def request_set_expression_json(self, py_db, request, thread_id):
        py_db.post_method_as_internal_command(
            thread_id, internal_set_expression_json, request, thread_id)

    def request_console_exec(self, py_db, seq, thread_id, frame_id, expression):
        int_cmd = InternalConsoleExec(seq, thread_id, frame_id, expression)
        py_db.post_internal_command(int_cmd, thread_id)

    def request_load_source(self, py_db, seq, filename):
        '''
        :param str filename:
            Note: must be sent as it was received in the protocol. It may be translated in this
            function.
        '''
        try:
            filename = self.filename_to_server(filename)
            assert filename.__class__ == str  # i.e.: bytes on py2 and str on py3

            with open(filename, 'r') as stream:
                source = stream.read()
            cmd = py_db.cmd_factory.make_load_source_message(seq, source)
        except:
            cmd = py_db.cmd_factory.make_error_message(seq, get_exception_traceback_str())

        py_db.writer.add_command(cmd)

    def add_python_exception_breakpoint(
            self,
            py_db,
            exception,
            condition,
            expression,
            notify_on_handled_exceptions,
            notify_on_unhandled_exceptions,
            notify_on_first_raise_only,
            ignore_libraries,
        ):
        exception_breakpoint = py_db.add_break_on_exception(
            exception,
            condition=condition,
            expression=expression,
            notify_on_handled_exceptions=notify_on_handled_exceptions,
            notify_on_unhandled_exceptions=notify_on_unhandled_exceptions,
            notify_on_first_raise_only=notify_on_first_raise_only,
            ignore_libraries=ignore_libraries,
        )

        if exception_breakpoint is not None:
            py_db.on_breakpoints_changed()

    def add_plugins_exception_breakpoint(self, py_db, breakpoint_type, exception):
        supported_type = False
        plugin = py_db.get_plugin_lazy_init()
        if plugin is not None:
            supported_type = plugin.add_breakpoint('add_exception_breakpoint', py_db, breakpoint_type, exception)

        if supported_type:
            py_db.has_plugin_exception_breaks = py_db.plugin.has_exception_breaks()
            py_db.on_breakpoints_changed()
        else:
            raise NameError(breakpoint_type)

    def remove_python_exception_breakpoint(self, py_db, exception):
        try:
            cp = py_db.break_on_uncaught_exceptions.copy()
            cp.pop(exception, None)
            py_db.break_on_uncaught_exceptions = cp

            cp = py_db.break_on_caught_exceptions.copy()
            cp.pop(exception, None)
            py_db.break_on_caught_exceptions = cp
        except:
            pydev_log.exception("Error while removing exception %s", sys.exc_info()[0])

        py_db.on_breakpoints_changed(removed=True)

    def remove_plugins_exception_breakpoint(self, py_db, exception_type, exception):
        # I.e.: no need to initialize lazy (if we didn't have it in the first place, we can't remove
        # anything from it anyways).
        plugin = py_db.plugin
        if plugin is None:
            return

        supported_type = plugin.remove_exception_breakpoint(py_db, exception_type, exception)

        if supported_type:
            py_db.has_plugin_exception_breaks = py_db.plugin.has_exception_breaks()
        else:
            raise NameError(exception_type)

        py_db.on_breakpoints_changed(removed=True)

    def remove_all_exception_breakpoints(self, py_db):
        py_db.break_on_uncaught_exceptions = {}
        py_db.break_on_caught_exceptions = {}

        plugin = py_db.plugin
        if plugin is not None:
            plugin.remove_all_exception_breakpoints(py_db)
        py_db.on_breakpoints_changed(removed=True)

    def set_project_roots(self, py_db, project_roots):
        '''
        :param unicode project_roots:
        '''
        py_db.set_project_roots(project_roots)

    def set_stepping_resumes_all_threads(self, py_db, stepping_resumes_all_threads):
        py_db.stepping_resumes_all_threads = stepping_resumes_all_threads

    # Add it to the namespace so that it's available as PyDevdAPI.ExcludeFilter
    from _pydevd_bundle.pydevd_filtering import ExcludeFilter  # noqa

    def set_exclude_filters(self, py_db, exclude_filters):
        '''
        :param list(PyDevdAPI.ExcludeFilter) exclude_filters:
        '''
        py_db.set_exclude_filters(exclude_filters)

    def set_use_libraries_filter(self, py_db, use_libraries_filter):
        py_db.set_use_libraries_filter(use_libraries_filter)

    def request_get_variable_json(self, py_db, request, thread_id):
        '''
        :param VariablesRequest request:
        '''
        py_db.post_method_as_internal_command(
            thread_id, internal_get_variable_json, request)

    def request_change_variable_json(self, py_db, request, thread_id):
        '''
        :param SetVariableRequest request:
        '''
        py_db.post_method_as_internal_command(
            thread_id, internal_change_variable_json, request)

    def set_dont_trace_start_end_patterns(self, py_db, start_patterns, end_patterns):
        # After it's set the first time, we can still change it, but we need to reset the
        # related caches.
        reset_caches = False
        dont_trace_start_end_patterns_previously_set = \
            py_db.dont_trace_external_files.__name__ == 'custom_dont_trace_external_files'

        if not dont_trace_start_end_patterns_previously_set and not start_patterns and not end_patterns:
            # If it wasn't set previously and start and end patterns are empty we don't need to do anything.
            return

        if not py_db.is_cache_file_type_empty():
            # i.e.: custom function set in set_dont_trace_start_end_patterns.
            if dont_trace_start_end_patterns_previously_set:
                reset_caches = py_db.dont_trace_external_files.start_patterns != start_patterns or \
                    py_db.dont_trace_external_files.end_patterns != end_patterns

            else:
                reset_caches = True

        def custom_dont_trace_external_files(abs_path):
            return abs_path.startswith(start_patterns) or abs_path.endswith(end_patterns)

        custom_dont_trace_external_files.start_patterns = start_patterns
        custom_dont_trace_external_files.end_patterns = end_patterns
        py_db.dont_trace_external_files = custom_dont_trace_external_files

        if reset_caches:
            py_db.clear_dont_trace_start_end_patterns_caches()

    def stop_on_entry(self):
        main_thread = pydevd_utils.get_main_thread()
        if main_thread is None:
            pydev_log.critical('Could not find main thread while setting Stop on Entry.')
        else:
            info = set_additional_thread_info(main_thread)
            info.pydev_original_step_cmd = CMD_STOP_ON_START
            info.pydev_step_cmd = CMD_STEP_INTO_MY_CODE

    def set_ignore_system_exit_codes(self, py_db, ignore_system_exit_codes):
        py_db.set_ignore_system_exit_codes(ignore_system_exit_codes)

    SourceMappingEntry = pydevd_source_mapping.SourceMappingEntry

    def set_source_mapping(self, py_db, source_filename, mapping):
        '''
        :param str source_filename:
            The filename for the source mapping (bytes on py2 and str on py3).

        :param list(SourceMappingEntry) mapping:
            A list with the source mapping entries to be applied to the given filename.

        :return str:
            An error message if it was not possible to set the mapping or an empty string if
            everything is ok.
        '''
        source_filename = self.filename_to_server(source_filename)
        for map_entry in mapping:
            map_entry.source_filename = source_filename
        error_msg = py_db.source_mapping.set_source_mapping(source_filename, mapping)
        if error_msg:
            return error_msg

        self.reapply_breakpoints(py_db)
        return ''

    def get_ppid(self):
        '''
        Provides the parent pid (even for older versions of Python on Windows).
        '''
        ppid = None

        try:
            ppid = os.getppid()
        except AttributeError:
            pass

        if ppid is None and IS_WINDOWS:
            ppid = self._get_windows_ppid()

        return ppid

    def _get_windows_ppid(self):
        this_pid = os.getpid()
        for ppid, pid in _list_ppid_and_pid():
            if pid == this_pid:
                return ppid

        return None

    def _terminate_child_processes_windows(self, dont_terminate_child_pids):
        this_pid = os.getpid()
        for _ in range(50):  # Try this at most 50 times before giving up.

            # Note: we can't kill the process itself with taskkill, so, we
            # list immediate children, kill that tree and then exit this process.

            children_pids = []
            for ppid, pid in _list_ppid_and_pid():
                if ppid == this_pid:
                    if pid not in dont_terminate_child_pids:
                        children_pids.append(pid)

            if not children_pids:
                break
            else:
                for pid in children_pids:
                    self._call(
                        ['taskkill', '/F', '/PID', str(pid), '/T'],
                        stdout=subprocess.PIPE,
                        stderr=subprocess.PIPE
                    )

                del children_pids[:]

    def _terminate_child_processes_linux_and_mac(self, dont_terminate_child_pids):
        this_pid = os.getpid()

        def list_children_and_stop_forking(initial_pid, stop=True):
            children_pids = []
            if stop:
                # Ask to stop forking (shouldn't be called for this process, only subprocesses).
                self._call(
                    ['kill', '-STOP', str(initial_pid)],
                    stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE
                )

            list_popen = self._popen(
                ['pgrep', '-P', str(initial_pid)],
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE
            )

            if list_popen is not None:
                stdout, _ = list_popen.communicate()
                for line in stdout.splitlines():
                    line = line.decode('ascii').strip()
                    if line:
                        pid = str(line)
                        if pid in dont_terminate_child_pids:
                            continue
                        children_pids.append(pid)
                        # Recursively get children.
                        children_pids.extend(list_children_and_stop_forking(pid))
            return children_pids

        previously_found = set()

        for _ in range(50):  # Try this at most 50 times before giving up.

            children_pids = list_children_and_stop_forking(this_pid, stop=False)
            found_new = False

            for pid in children_pids:
                if pid not in previously_found:
                    found_new = True
                    previously_found.add(pid)
                    self._call(
                        ['kill', '-KILL', str(pid)],
                        stdout=subprocess.PIPE,
                        stderr=subprocess.PIPE
                    )

            if not found_new:
                break

    def _popen(self, cmdline, **kwargs):
        try:
            return subprocess.Popen(cmdline, **kwargs)
        except:
            if DebugInfoHolder.DEBUG_TRACE_LEVEL >= 1:
                pydev_log.exception('Error running: %s' % (' '.join(cmdline)))
            return None

    def _call(self, cmdline, **kwargs):
        try:
            subprocess.check_call(cmdline, **kwargs)
        except:
            if DebugInfoHolder.DEBUG_TRACE_LEVEL >= 1:
                pydev_log.exception('Error running: %s' % (' '.join(cmdline)))

    def set_terminate_child_processes(self, py_db, terminate_child_processes):
        py_db.terminate_child_processes = terminate_child_processes

    def terminate_process(self, py_db):
        '''
        Terminates the current process (and child processes if the option to also terminate
        child processes is enabled).
        '''
        try:
            if py_db.terminate_child_processes:
                pydev_log.debug('Terminating child processes.')
                if IS_WINDOWS:
                    self._terminate_child_processes_windows(py_db.dont_terminate_child_pids)
                else:
                    self._terminate_child_processes_linux_and_mac(py_db.dont_terminate_child_pids)
        finally:
            pydev_log.debug('Exiting process (os._exit(0)).')
            os._exit(0)

    def _terminate_if_commands_processed(self, py_db):
        py_db.dispose_and_kill_all_pydevd_threads()
        self.terminate_process(py_db)

    def request_terminate_process(self, py_db):
        # We mark with a terminate_requested to avoid that paused threads start running
        # (we should terminate as is without letting any paused thread run).
        py_db.terminate_requested = True
        run_as_pydevd_daemon_thread(py_db, self._terminate_if_commands_processed, py_db)


def _list_ppid_and_pid():
    _TH32CS_SNAPPROCESS = 0x00000002

    class PROCESSENTRY32(ctypes.Structure):
        _fields_ = [("dwSize", ctypes.c_uint32),
                    ("cntUsage", ctypes.c_uint32),
                    ("th32ProcessID", ctypes.c_uint32),
                    ("th32DefaultHeapID", ctypes.c_size_t),
                    ("th32ModuleID", ctypes.c_uint32),
                    ("cntThreads", ctypes.c_uint32),
                    ("th32ParentProcessID", ctypes.c_uint32),
                    ("pcPriClassBase", ctypes.c_long),
                    ("dwFlags", ctypes.c_uint32),
                    ("szExeFile", ctypes.c_char * 260)]

    kernel32 = ctypes.windll.kernel32
    snapshot = kernel32.CreateToolhelp32Snapshot(_TH32CS_SNAPPROCESS, 0)
    ppid_and_pids = []
    try:
        process_entry = PROCESSENTRY32()
        process_entry.dwSize = ctypes.sizeof(PROCESSENTRY32)
        if not kernel32.Process32First(snapshot, ctypes.byref(process_entry)):
            pydev_log.critical('Process32First failed (getting process from CreateToolhelp32Snapshot).')
        else:
            while True:
                ppid_and_pids.append((process_entry.th32ParentProcessID, process_entry.th32ProcessID))
                if not kernel32.Process32Next(snapshot, ctypes.byref(process_entry)):
                    break
    finally:
        kernel32.CloseHandle(snapshot)
        
    return ppid_and_pids
