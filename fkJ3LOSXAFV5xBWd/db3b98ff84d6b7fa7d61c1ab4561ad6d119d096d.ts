import Config = require("./Config");
import Context = require("./Context");
import * as Contracts from "../Declarations/Contracts";
/** State Container for sending to the QuickPulse Service */
declare class QuickPulseStateManager {
    config: Config;
    context: Context;
    private static MAX_POST_WAIT_TIME;
    private static MAX_PING_WAIT_TIME;
    private static FALLBACK_INTERVAL;
    private static PING_INTERVAL;
    private static POST_INTERVAL;
    private _isCollectingData;
    private _sender;
    private _isEnabled;
    private _lastSuccessTime;
    private _lastSendSucceeded;
    private _handle;
    private _metrics;
    private _documents;
    private _collectors;
    constructor(iKey?: string, context?: Context);
    /**
     *
     * @param collector
     */
    addCollector(collector: any): void;
    /**
     * Override of TelemetryClient.trackMetric
     */
    trackMetric(telemetry: Contracts.MetricTelemetry): void;
    /**
     * Add a document to the current buffer
     * @param envelope
     */
    addDocument(envelope: Contracts.Envelope): void;
    /**
     * Enable or disable communication with QuickPulseService
     * @param isEnabled
     */
    enable(isEnabled: boolean): void;
    /**
     * Enable or disable all collectors in this instance
     * @param enable
     */
    private enableCollectors(enable);
    /**
     * Add the metric to this buffer. If same metric already exists in this buffer, add weight to it
     * @param telemetry
     */
    private _addMetric(telemetry);
    private _resetQuickPulseBuffer();
    private _goQuickPulse();
    private _ping(envelope);
    private _post(envelope);
    /**
     * Change the current QPS send state. (shouldPOST == undefined) --> error, but do not change the state yet.
     */
    private _quickPulseDone(shouldPOST?, res?);
}
export = QuickPulseStateManager;
