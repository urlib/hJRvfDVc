module.exports=function(e){var t={};function r(n){if(t[n])return t[n].exports;var o=t[n]={i:n,l:!1,exports:{}};return e[n].call(o.exports,o,o.exports,r),o.l=!0,o.exports}return r.m=e,r.c=t,r.d=function(e,t,n){r.o(e,t)||Object.defineProperty(e,t,{enumerable:!0,get:n})},r.r=function(e){"undefined"!=typeof Symbol&&Symbol.toStringTag&&Object.defineProperty(e,Symbol.toStringTag,{value:"Module"}),Object.defineProperty(e,"__esModule",{value:!0})},r.t=function(e,t){if(1&t&&(e=r(e)),8&t)return e;if(4&t&&"object"==typeof e&&e&&e.__esModule)return e;var n=Object.create(null);if(r.r(n),Object.defineProperty(n,"default",{enumerable:!0,value:e}),2&t&&"string"!=typeof e)for(var o in e)r.d(n,o,function(t){return e[t]}.bind(null,o));return n},r.n=function(e){var t=e&&e.__esModule?function(){return e.default}:function(){return e};return r.d(t,"a",t),t},r.o=function(e,t){return Object.prototype.hasOwnProperty.call(e,t)},r.p="",r(r.s=893)}({893:function(e,t,r){"use strict";var n=r(894);e.exports=function(e,t){var r;if(e.progressContext)return e;if(e.response)throw new Error("Already got response, it's too late to track progress");return(t=t||{}).throttle=null==t.throttle?1e3:t.throttle,t.delay=t.delay||0,t.lengthHeader=t.lengthHeader||"content-length",(r={}).request=e,r.options=t,r.reportState=n(function(e){var t;!e.delayTimer&&e.request.progressState&&((t=e.state).time.elapsed=(Date.now()-e.startedAt)/1e3,t.time.elapsed>=1&&(t.speed=t.size.transferred/t.time.elapsed),null!=t.size.total&&(t.percent=Math.min(t.size.transferred,t.size.total)/t.size.total,null!=t.speed&&(t.time.remaining=1!==t.percent?t.size.total/t.speed-t.time.elapsed:0,t.time.remaining=Math.round(1e3*t.time.remaining)/1e3)),e.request.emit("progress",t))}.bind(null,r),t.throttle),e.on("request",function(e){e.startedAt=null,e.state=e.request.progressState=null,e.delayTimer&&clearTimeout(e.delayTimer),e.delayTimer=null}.bind(null,r)).on("response",function(e){return e.on("data",function(e,t){e.state.size.transferred+=t.length,!e.delayTimer&&e.reportState()}.bind(null,r)),function(e,t){e.startedAt=Date.now(),e.state=e.request.progressState={time:{elapsed:0,remaining:null},speed:null,percent:null,size:{total:Number(t.headers[e.options.lengthHeader])||null,transferred:0}},e.delayTimer=setTimeout(function(){e.delayTimer=null},e.options.delay)}(r,e)}).on("end",function(e){e.delayTimer&&(clearTimeout(e.delayTimer),e.delayTimer=null),e.request.progressState=e.request.progressContext=null}.bind(null,r)),e.progressContext=r,e}},894:function(e,t){e.exports=function(e,t){var r,n,o,l,i=0;return function(){r=this,n=arguments;var e=new Date-i;return l||(e>=t?u():l=setTimeout(u,t-e)),o};function u(){l=0,i=+new Date,o=e.apply(r,n),r=null,n=null}}}});
//# sourceMappingURL=request-progress.js.map