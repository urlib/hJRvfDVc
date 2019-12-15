import CorrelationContextManager = require("./AutoCollection/CorrelationContextManager");
import QuickPulseClient = require("./Library/QuickPulseStateManager");
import { IDisabledExtendedMetrics } from "./AutoCollection/NativePerformance";
export import TelemetryClient = require("./Library/NodeClient");
export import Contracts = require("./Declarations/Contracts");
export declare enum DistributedTracingModes {
    /**
     * (Default) Send Application Insights correlation headers
     */
    AI = 0,
    /**
     * Send both W3C Trace Context headers and back-compatibility Application Insights headers
     */
    AI_AND_W3C = 1,
}
/**
* The default client, initialized when setup was called. To initialize a different client
* with its own configuration, use `new TelemetryClient(instrumentationKey?)`.
*/
export declare let defaultClient: TelemetryClient;
export declare let liveMetricsClient: QuickPulseClient;
/**
 * Initializes the default client. Should be called after setting
 * configuration options.
 *
 * @param instrumentationKey the instrumentation key to use. Optional, if
 * this is not specified, the value will be read from the environment
 * variable APPINSIGHTS_INSTRUMENTATIONKEY.
 * @returns {Configuration} the configuration class to initialize
 * and start the SDK.
 */
export declare function setup(instrumentationKey?: string): typeof Configuration;
/**
 * Starts automatic collection of telemetry. Prior to calling start no
 * telemetry will be *automatically* collected, though manual collection
 * is enabled.
 * @returns {ApplicationInsights} this class
 */
export declare function start(): typeof Configuration;
/**
 * Returns an object that is shared across all code handling a given request.
 * This can be used similarly to thread-local storage in other languages.
 * Properties set on this object will be available to telemetry processors.
 *
 * Do not store sensitive information here.
 * Custom properties set on this object can be exposed in a future SDK
 * release via outgoing HTTP headers.
 * This is to allow for correlating data cross-component.
 *
 * This method will return null if automatic dependency correlation is disabled.
 * @returns A plain object for request storage or null if automatic dependency correlation is disabled.
 */
export declare function getCorrelationContext(): CorrelationContextManager.CorrelationContext;
/**
 * Returns a function that will get the same correlation context within its
 * function body as the code executing this function.
 * Use this method if automatic dependency correlation is not propagating
 * correctly to an asynchronous callback.
 */
export declare function wrapWithCorrelationContext<T extends Function>(fn: T): T;
/**
 * The active configuration for global SDK behaviors, such as autocollection.
 */
export declare class Configuration {
    static start: typeof start;
    /**
     * Sets the distributed tracing modes. If W3C mode is enabled, W3C trace context
     * headers (traceparent/tracestate) will be parsed in all incoming requests, and included in outgoing
     * requests. In W3C mode, existing back-compatibility AI headers will also be parsed and included.
     * Enabling W3C mode will not break existing correlation with other Application Insights instrumented
     * services. Default=AI
    */
    static setDistributedTracingMode(value: DistributedTracingModes): typeof Configuration;
    /**
     * Sets the state of console and logger tracking (enabled by default for third-party loggers only)
     * @param value if true logger activity will be sent to Application Insights
     * @param collectConsoleLog if true, logger autocollection will include console.log calls (default false)
     * @returns {Configuration} this class
     */
    static setAutoCollectConsole(value: boolean, collectConsoleLog?: boolean): typeof Configuration;
    /**
     * Sets the state of exception tracking (enabled by default)
     * @param value if true uncaught exceptions will be sent to Application Insights
     * @returns {Configuration} this class
     */
    static setAutoCollectExceptions(value: boolean): typeof Configuration;
    /**
     * Sets the state of performance tracking (enabled by default)
     * @param value if true performance counters will be collected every second and sent to Application Insights
     * @param collectExtendedMetrics if true, extended metrics counters will be collected every minute and sent to Application Insights
     * @returns {Configuration} this class
     */
    static setAutoCollectPerformance(value: boolean, collectExtendedMetrics?: boolean | IDisabledExtendedMetrics): typeof Configuration;
    /**
     * Sets the state of request tracking (enabled by default)
     * @param value if true requests will be sent to Application Insights
     * @returns {Configuration} this class
     */
    static setAutoCollectRequests(value: boolean): typeof Configuration;
    /**
     * Sets the state of dependency tracking (enabled by default)
     * @param value if true dependencies will be sent to Application Insights
     * @returns {Configuration} this class
     */
    static setAutoCollectDependencies(value: boolean): typeof Configuration;
    /**
     * Sets the state of automatic dependency correlation (enabled by default)
     * @param value if true dependencies will be correlated with requests
     * @param useAsyncHooks if true, forces use of experimental async_hooks module to provide correlation. If false, instead uses only patching-based techniques. If left blank, the best option is chosen for you based on your version of Node.js.
     * @returns {Configuration} this class
     */
    static setAutoDependencyCorrelation(value: boolean, useAsyncHooks?: boolean): typeof Configuration;
    /**
     * Enable or disable disk-backed retry caching to cache events when client is offline (enabled by default)
     * Note that this method only applies to the default client. Disk-backed retry caching is disabled by default for additional clients.
     * For enable for additional clients, use client.channel.setUseDiskRetryCaching(true).
     * These cached events are stored in your system or user's temporary directory and access restricted to your user when possible.
     * @param value if true events that occured while client is offline will be cached on disk
     * @param resendInterval The wait interval for resending cached events.
     * @param maxBytesOnDisk The maximum size (in bytes) that the created temporary directory for cache events can grow to, before caching is disabled.
     * @returns {Configuration} this class
     */
    static setUseDiskRetryCaching(value: boolean, resendInterval?: number, maxBytesOnDisk?: number): typeof Configuration;
    /**
     * Enables debug and warning logging for AppInsights itself.
     * @param enableDebugLogging if true, enables debug logging
     * @param enableWarningLogging if true, enables warning logging
     * @returns {Configuration} this class
     */
    static setInternalLogging(enableDebugLogging?: boolean, enableWarningLogging?: boolean): typeof Configuration;
    /**
     * Enables communication with Application Insights Live Metrics.
     * @param enable if true, enables communication with the live metrics service
     */
    static setSendLiveMetrics(enable?: boolean): typeof Configuration;
}
/**
 * Disposes the default client and all the auto collectors so they can be reinitialized with different configuration
*/
export declare function dispose(): void;
