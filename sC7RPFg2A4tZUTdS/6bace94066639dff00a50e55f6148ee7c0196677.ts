import { DebugProtocol } from 'vscode-debugprotocol';
import { ProtocolServer } from './protocol';
import { Event } from './messages';
export declare class Source implements DebugProtocol.Source {
    name: string;
    path: string;
    sourceReference: number;
    constructor(name: string, path?: string, id?: number, origin?: string, data?: any);
}
export declare class Scope implements DebugProtocol.Scope {
    name: string;
    variablesReference: number;
    expensive: boolean;
    constructor(name: string, reference: number, expensive?: boolean);
}
export declare class StackFrame implements DebugProtocol.StackFrame {
    id: number;
    source: Source;
    line: number;
    column: number;
    name: string;
    constructor(i: number, nm: string, src?: Source, ln?: number, col?: number);
}
export declare class Thread implements DebugProtocol.Thread {
    id: number;
    name: string;
    constructor(id: number, name: string);
}
export declare class Variable implements DebugProtocol.Variable {
    name: string;
    value: string;
    variablesReference: number;
    constructor(name: string, value: string, ref?: number, indexedVariables?: number, namedVariables?: number);
}
export declare class Breakpoint implements DebugProtocol.Breakpoint {
    verified: boolean;
    constructor(verified: boolean, line?: number, column?: number, source?: Source);
}
export declare class Module implements DebugProtocol.Module {
    id: number | string;
    name: string;
    constructor(id: number | string, name: string);
}
export declare class CompletionItem implements DebugProtocol.CompletionItem {
    label: string;
    start: number;
    length: number;
    constructor(label: string, start: number, length?: number);
}
export declare class StoppedEvent extends Event implements DebugProtocol.StoppedEvent {
    body: {
        reason: string;
    };
    constructor(reason: string, threadId?: number, exceptionText?: string);
}
export declare class ContinuedEvent extends Event implements DebugProtocol.ContinuedEvent {
    body: {
        threadId: number;
    };
    constructor(threadId: number, allThreadsContinued?: boolean);
}
export declare class InitializedEvent extends Event implements DebugProtocol.InitializedEvent {
    constructor();
}
export declare class TerminatedEvent extends Event implements DebugProtocol.TerminatedEvent {
    constructor(restart?: any);
}
export declare class OutputEvent extends Event implements DebugProtocol.OutputEvent {
    body: {
        category: string;
        output: string;
        data?: any;
    };
    constructor(output: string, category?: string, data?: any);
}
export declare class ThreadEvent extends Event implements DebugProtocol.ThreadEvent {
    body: {
        reason: string;
        threadId: number;
    };
    constructor(reason: string, threadId: number);
}
export declare class BreakpointEvent extends Event implements DebugProtocol.BreakpointEvent {
    body: {
        reason: string;
        breakpoint: Breakpoint;
    };
    constructor(reason: string, breakpoint: Breakpoint);
}
export declare class ModuleEvent extends Event implements DebugProtocol.ModuleEvent {
    body: {
        reason: 'new' | 'changed' | 'removed';
        module: Module;
    };
    constructor(reason: 'new' | 'changed' | 'removed', module: Module);
}
export declare class LoadedSourceEvent extends Event implements DebugProtocol.LoadedSourceEvent {
    body: {
        reason: 'new' | 'changed' | 'removed';
        source: Source;
    };
    constructor(reason: 'new' | 'changed' | 'removed', source: Source);
}
export declare class CapabilitiesEvent extends Event implements DebugProtocol.CapabilitiesEvent {
    body: {
        capabilities: DebugProtocol.Capabilities;
    };
    constructor(capabilities: DebugProtocol.Capabilities);
}
export declare enum ErrorDestination {
    User = 1,
    Telemetry = 2
}
export declare class DebugSession extends ProtocolServer {
    private _debuggerLinesStartAt1;
    private _debuggerColumnsStartAt1;
    private _debuggerPathsAreURIs;
    private _clientLinesStartAt1;
    private _clientColumnsStartAt1;
    private _clientPathsAreURIs;
    protected _isServer: boolean;
    constructor(obsolete_debuggerLinesAndColumnsStartAt1?: boolean, obsolete_isServer?: boolean);
    setDebuggerPathFormat(format: string): void;
    setDebuggerLinesStartAt1(enable: boolean): void;
    setDebuggerColumnsStartAt1(enable: boolean): void;
    setRunAsServer(enable: boolean): void;
    /**
     * A virtual constructor...
     */
    static run(debugSession: typeof DebugSession): void;
    shutdown(): void;
    protected sendErrorResponse(response: DebugProtocol.Response, codeOrMessage: number | DebugProtocol.Message, format?: string, variables?: any, dest?: ErrorDestination): void;
    runInTerminalRequest(args: DebugProtocol.RunInTerminalRequestArguments, timeout: number, cb: (response: DebugProtocol.RunInTerminalResponse) => void): void;
    protected dispatchRequest(request: DebugProtocol.Request): void;
    protected initializeRequest(response: DebugProtocol.InitializeResponse, args: DebugProtocol.InitializeRequestArguments): void;
    protected disconnectRequest(response: DebugProtocol.DisconnectResponse, args: DebugProtocol.DisconnectArguments): void;
    protected launchRequest(response: DebugProtocol.LaunchResponse, args: DebugProtocol.LaunchRequestArguments): void;
    protected attachRequest(response: DebugProtocol.AttachResponse, args: DebugProtocol.AttachRequestArguments): void;
    protected terminateRequest(response: DebugProtocol.TerminateResponse, args: DebugProtocol.TerminateArguments): void;
    protected restartRequest(response: DebugProtocol.RestartResponse, args: DebugProtocol.RestartArguments): void;
    protected setBreakPointsRequest(response: DebugProtocol.SetBreakpointsResponse, args: DebugProtocol.SetBreakpointsArguments): void;
    protected setFunctionBreakPointsRequest(response: DebugProtocol.SetFunctionBreakpointsResponse, args: DebugProtocol.SetFunctionBreakpointsArguments): void;
    protected setExceptionBreakPointsRequest(response: DebugProtocol.SetExceptionBreakpointsResponse, args: DebugProtocol.SetExceptionBreakpointsArguments): void;
    protected configurationDoneRequest(response: DebugProtocol.ConfigurationDoneResponse, args: DebugProtocol.ConfigurationDoneArguments): void;
    protected continueRequest(response: DebugProtocol.ContinueResponse, args: DebugProtocol.ContinueArguments): void;
    protected nextRequest(response: DebugProtocol.NextResponse, args: DebugProtocol.NextArguments): void;
    protected stepInRequest(response: DebugProtocol.StepInResponse, args: DebugProtocol.StepInArguments): void;
    protected stepOutRequest(response: DebugProtocol.StepOutResponse, args: DebugProtocol.StepOutArguments): void;
    protected stepBackRequest(response: DebugProtocol.StepBackResponse, args: DebugProtocol.StepBackArguments): void;
    protected reverseContinueRequest(response: DebugProtocol.ReverseContinueResponse, args: DebugProtocol.ReverseContinueArguments): void;
    protected restartFrameRequest(response: DebugProtocol.RestartFrameResponse, args: DebugProtocol.RestartFrameArguments): void;
    protected gotoRequest(response: DebugProtocol.GotoResponse, args: DebugProtocol.GotoArguments): void;
    protected pauseRequest(response: DebugProtocol.PauseResponse, args: DebugProtocol.PauseArguments): void;
    protected sourceRequest(response: DebugProtocol.SourceResponse, args: DebugProtocol.SourceArguments): void;
    protected threadsRequest(response: DebugProtocol.ThreadsResponse): void;
    protected terminateThreadsRequest(response: DebugProtocol.TerminateThreadsResponse, args: DebugProtocol.TerminateThreadsRequest): void;
    protected stackTraceRequest(response: DebugProtocol.StackTraceResponse, args: DebugProtocol.StackTraceArguments): void;
    protected scopesRequest(response: DebugProtocol.ScopesResponse, args: DebugProtocol.ScopesArguments): void;
    protected variablesRequest(response: DebugProtocol.VariablesResponse, args: DebugProtocol.VariablesArguments): void;
    protected setVariableRequest(response: DebugProtocol.SetVariableResponse, args: DebugProtocol.SetVariableArguments): void;
    protected setExpressionRequest(response: DebugProtocol.SetExpressionResponse, args: DebugProtocol.SetExpressionArguments): void;
    protected evaluateRequest(response: DebugProtocol.EvaluateResponse, args: DebugProtocol.EvaluateArguments): void;
    protected stepInTargetsRequest(response: DebugProtocol.StepInTargetsResponse, args: DebugProtocol.StepInTargetsArguments): void;
    protected gotoTargetsRequest(response: DebugProtocol.GotoTargetsResponse, args: DebugProtocol.GotoTargetsArguments): void;
    protected completionsRequest(response: DebugProtocol.CompletionsResponse, args: DebugProtocol.CompletionsArguments): void;
    protected exceptionInfoRequest(response: DebugProtocol.ExceptionInfoResponse, args: DebugProtocol.ExceptionInfoArguments): void;
    protected loadedSourcesRequest(response: DebugProtocol.LoadedSourcesResponse, args: DebugProtocol.LoadedSourcesArguments): void;
    protected dataBreakpointInfoRequest(response: DebugProtocol.DataBreakpointInfoResponse, args: DebugProtocol.DataBreakpointInfoArguments): void;
    protected setDataBreakpointsRequest(response: DebugProtocol.SetDataBreakpointsResponse, args: DebugProtocol.SetDataBreakpointsArguments): void;
    protected readMemoryRequest(response: DebugProtocol.ReadMemoryResponse, args: DebugProtocol.ReadMemoryArguments): void;
    protected disassembleRequest(response: DebugProtocol.DisassembleResponse, args: DebugProtocol.DisassembleArguments): void;
    /**
     * Override this hook to implement custom requests.
     */
    protected customRequest(command: string, response: DebugProtocol.Response, args: any): void;
    protected convertClientLineToDebugger(line: number): number;
    protected convertDebuggerLineToClient(line: number): number;
    protected convertClientColumnToDebugger(column: number): number;
    protected convertDebuggerColumnToClient(column: number): number;
    protected convertClientPathToDebugger(clientPath: string): string;
    protected convertDebuggerPathToClient(debuggerPath: string): string;
    private static path2uri;
    private static uri2path;
    private static _formatPIIRegexp;
    private static formatPII;
}
