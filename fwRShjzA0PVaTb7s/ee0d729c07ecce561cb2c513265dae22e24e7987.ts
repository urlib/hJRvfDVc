/**
 * @license
 * Copyright Google Inc. All Rights Reserved.
 *
 * Use of this source code is governed by an MIT-style license that can be
 * found in the LICENSE file at https://angular.io/license
 */

import {patchEventTargetMethods} from '../common/utils';

const WTF_ISSUE_555 =
    'Anchor,Area,Audio,BR,Base,BaseFont,Body,Button,Canvas,Content,DList,Directory,Div,Embed,FieldSet,Font,Form,Frame,FrameSet,HR,Head,Heading,Html,IFrame,Image,Input,Keygen,LI,Label,Legend,Link,Map,Marquee,Media,Menu,Meta,Meter,Mod,OList,Object,OptGroup,Option,Output,Paragraph,Pre,Progress,Quote,Script,Select,Source,Span,Style,TableCaption,TableCell,TableCol,Table,TableRow,TableSection,TextArea,Title,Track,UList,Unknown,Video';
const NO_EVENT_TARGET =
    'ApplicationCache,EventSource,FileReader,InputMethodContext,MediaController,MessagePort,Node,Performance,SVGElementInstance,SharedWorker,TextTrack,TextTrackCue,TextTrackList,WebKitNamedFlow,Window,Worker,WorkerGlobalScope,XMLHttpRequest,XMLHttpRequestEventTarget,XMLHttpRequestUpload,IDBRequest,IDBOpenDBRequest,IDBDatabase,IDBTransaction,IDBCursor,DBIndex,WebSocket'
        .split(',');
const EVENT_TARGET = 'EventTarget';

export function eventTargetPatch(_global) {
  let apis = [];
  const isWtf = _global['wtf'];
  if (isWtf) {
    // Workaround for: https://github.com/google/tracing-framework/issues/555
    apis = WTF_ISSUE_555.split(',').map((v) => 'HTML' + v + 'Element').concat(NO_EVENT_TARGET);
  } else if (_global[EVENT_TARGET]) {
    apis.push(EVENT_TARGET);
  } else {
    // Note: EventTarget is not available in all browsers,
    // if it's not available, we instead patch the APIs in the IDL that inherit from EventTarget
    apis = NO_EVENT_TARGET;
  }

  for (let i = 0; i < apis.length; i++) {
    const type = _global[apis[i]];
    patchEventTargetMethods(type && type.prototype);
  }
}
