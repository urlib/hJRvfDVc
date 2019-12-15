import * as vscode from 'vscode';
import { CancellationToken } from 'vscode-jsonrpc';
/**
 * API version information.
 */
export declare enum Version {
    v0 = 0,
    v1 = 1,
    v2 = 2,
    v3 = 3,
    latest = 3,
}
/**
 * An interface to allow VS Code extensions to communicate with the C/C++ extension.
 * @see [CppToolsExtension](#CppToolsExtension) for a code example.
 */
export interface CppToolsApi extends vscode.Disposable {
    /**
     * The version of the API being used.
     */
    getVersion(): Version;
    /**
     * Register a [CustomConfigurationProvider](#CustomConfigurationProvider).
     * This should be called as soon as the provider extension has been activated and determines that
     * it is capable of providing configurations for the workspace. The provider extension does not
     * need to be ready to provide configurations when this is called. The C/C++ extension will not
     * request configurations until the extension has signaled that it is ready to provide them.
     * @see [](#)
     * @param provider An instance of the [CustomConfigurationProvider](#CustomConfigurationProvider)
     * instance representing the provider extension.
     */
    registerCustomConfigurationProvider(provider: CustomConfigurationProvider): void;
    /**
     * Notify the C/C++ extension that the [CustomConfigurationProvider](#CustomConfigurationProvider)
     * is ready to provide custom configurations.
     * @param provider An instance of the [CustomConfigurationProvider](#CustomConfigurationProvider)
     * instance representing the provider extension.
     */
    notifyReady(provider: CustomConfigurationProvider): void;
    /**
     * Notify the C/C++ extension that the current configuration has changed. Upon receiving this
     * notification, the C/C++ extension will request the new configurations.
     * @param provider An instance of the [CustomConfigurationProvider](#CustomConfigurationProvider)
     * instance representing the provider extension.
     */
    didChangeCustomConfiguration(provider: CustomConfigurationProvider): void;
    /**
     * Notify the C/C++ extension that the code browsing configuration has changed. Upon receiving this
     * notification, the C/C++ extension will request the new configuration.
     * @param provider An instance of the [CustomConfigurationProvider](#CustomConfigurationProvider)
     * instance representing the provider extension.
     */
    didChangeCustomBrowseConfiguration(provider: CustomConfigurationProvider): void;
}
/**
 * An interface to allow this extension to communicate with Custom Configuration Provider extensions.
 */
export interface CustomConfigurationProvider extends vscode.Disposable {
    /**
     * The friendly name of the Custom Configuration Provider extension.
     */
    readonly name: string;
    /**
     * The id of the extension providing custom configurations. (e.g. `ms-vscode.cpptools`)
     */
    readonly extensionId: string;
    /**
     * A request to determine whether this provider can provide IntelliSense configurations for the given document.
     * @param uri The URI of the document.
     * @param token (optional) The cancellation token.
     * @returns 'true' if this provider can provide IntelliSense configurations for the given document.
     */
    canProvideConfiguration(uri: vscode.Uri, token?: CancellationToken): Thenable<boolean>;
    /**
     * A request to get Intellisense configurations for the given files.
     * @param uris A list of one of more URIs for the files to provide configurations for.
     * @param token (optional) The cancellation token.
     * @returns A list of [SourceFileConfigurationItem](#SourceFileConfigurationItem) for the documents that this provider
     * is able to provide IntelliSense configurations for.
     * Note: If this provider cannot provide configurations for any of the files in `uris`, the provider may omit the
     * configuration for that file in the return value. An empty array may be returned if the provider cannot provide
     * configurations for any of the files requested.
     */
    provideConfigurations(uris: vscode.Uri[], token?: CancellationToken): Thenable<SourceFileConfigurationItem[]>;
    /**
     * A request to determine whether this provider can provide a code browsing configuration for the workspace folder.
     * @param token (optional) The cancellation token.
     * @returns 'true' if this provider can provide a code browsing configuration for the workspace folder.
     */
    canProvideBrowseConfiguration(token?: CancellationToken): Thenable<boolean>;
    /**
     * A request to get the code browsing configuration for the workspace folder.
     * @param token (optional) The cancellation token.
     * @returns A [WorkspaceBrowseConfiguration](#WorkspaceBrowseConfiguration) with the information required to
     * construct the equivalent of `browse.path` from `c_cpp_properties.json`.
     */
    provideBrowseConfiguration(token?: CancellationToken): Thenable<WorkspaceBrowseConfiguration>;
    /**
     * A request to determine whether this provider can provide a code browsing configuration for each folder in a multi-root workspace.
     * @param token (optional) The cancellation token.
     * @returns 'true' if this provider can provide a code browsing configuration for each folder in a multi-root workspace.
     */
    canProvideBrowseConfigurationsPerFolder(token?: CancellationToken): Thenable<boolean>;
    /**
     * A request to get the code browsing configuration for the workspace folder.
     * @param uri The URI of the folder to provide a browse configuration for.
     * @param token (optional) The cancellation token.
     * @returns A [WorkspaceBrowseConfiguration](#WorkspaceBrowseConfiguration) with the information required to
     * construct the equivalent of `browse.path` from `c_cpp_properties.json`.
     */
    provideFolderBrowseConfiguration(uri: vscode.Uri, token?: CancellationToken): Thenable<WorkspaceBrowseConfiguration>;
}
/**
 * The model representing the custom IntelliSense configurations for a source file.
 */
export interface SourceFileConfiguration {
    /**
     * This must also include the system include path (compiler defaults) unless
     * [compilerPath](#SourceFileConfiguration.compilerPath) is specified.
     * Mac framework paths may also be added to this array.
     */
    readonly includePath: string[];
    /**
     * This must also include the compiler default defines (__cplusplus, etc) unless
     * [compilerPath](#SourceFileConfiguration.compilerPath) is specified.
     */
    readonly defines: string[];
    /**
     * The compiler to emulate.
     */
    readonly intelliSenseMode: "msvc-x86" | "msvc-x64" | "gcc-x86" | "gcc-x64" | "clang-x86" | "clang-x64";
    /**
     * The C or C++ standard to emulate.
     */
    readonly standard: "c89" | "c99" | "c11" | "c++98" | "c++03" | "c++11" | "c++14" | "c++17" | "c++20";
    /**
     * Any files that need to be included before the source file is parsed.
     */
    readonly forcedInclude?: string[];
    /**
     * The full path to the compiler. If specified, the extension will query it for system includes and defines and
     * add them to [includePath](#SourceFileConfiguration.includePath) and [defines](#SourceFileConfiguration.defines).
     */
    readonly compilerPath?: string;
    /**
     * Arguments for the compiler.
     */
    readonly compilerArgs?: string[];
    /**
     * The version of the Windows SDK that should be used. This field will only be used if
     * [compilerPath](#SourceFileConfiguration.compilerPath) is set and the compiler is capable of targeting Windows.
     */
    readonly windowsSdkVersion?: string;
}
/**
 * The model representing a source file and its corresponding configuration.
 */
export interface SourceFileConfigurationItem {
    /**
     * The URI of the source file. It should follow the file URI scheme and represent an absolute path to the file.
     * @example
```
    // When working with a file path,
    // convert it to a vscode.Uri first.
    let path: string = ...;
    let item: SourceFileConfigurationItem = {
        uri: vscode.Uri.file(path),
        configuration: ...
    };
```
     */
    readonly uri: string | vscode.Uri;
    /**
     * The IntelliSense configuration for [uri](#SourceFileConfigurationItem.uri)
     */
    readonly configuration: SourceFileConfiguration;
}
/**
 * The model representing the source browsing configuration for a workspace folder.
 */
export interface WorkspaceBrowseConfiguration {
    /**
     * This must also include the system include path (compiler defaults) unless
     * [compilerPath](#WorkspaceBrowseConfiguration.compilerPath) is specified.
     */
    readonly browsePath: string[];
    /**
     * The full path to the compiler. If specified, the extension will query it for system includes and
     * add them to [browsePath](#WorkspaceBrowseConfiguration.browsePath).
     */
    readonly compilerPath?: string;
    /**
     * Arguments for the compiler.
     */
    readonly compilerArgs?: string[];
    /**
     * The C or C++ standard to emulate. This field defaults to "c++17" and will only be used if
     * [compilerPath](#WorkspaceBrowseConfiguration.compilerPath) is set.
     */
    readonly standard?: "c89" | "c99" | "c11" | "c++98" | "c++03" | "c++11" | "c++14" | "c++17" | "c++20";
    /**
     * The version of the Windows SDK that should be used. This field defaults to the latest Windows SDK
     * installed on the PC and will only be used if [compilerPath](#WorkspaceBrowseConfiguration.compilerPath)
     * is set and the compiler is capable of targeting Windows.
     */
    readonly windowsSdkVersion?: string;
}
/**
 * The interface provided by the C/C++ extension during activation.
 * It is recommended to use the helper function [getCppToolsApi](#getCppToolsApi) instead
 * of querying the extension instance directly.
 */
export interface CppToolsExtension {
    /**
     * Get an API object.
     * @param version The desired version.
     */
    getApi(version: Version): CppToolsApi;
}
/**
 * Helper function to get the CppToolsApi from the cpptools extension.
 * @param version The desired API version
 * @example
```
    import {CppToolsApi, Version, CustomConfigurationProvider, getCppToolsApi} from 'vscode-cpptools';

    let api: CppToolsApi|undefined = await getCppToolsApi(Version.v1);
    if (api) {
        // Inform cpptools that a custom config provider
        // will be able to service the current workspace.
        api.registerCustomConfigurationProvider(provider);

        // Do any required setup that the provider needs.

        // Notify cpptools that the provider is ready to
        // provide IntelliSense configurations.
        api.notifyReady(provider);
    }
    // Dispose of the 'api' in your extension's
    // deactivate() method, or whenever you want to
    // unregister the provider.
```
 */
export declare function getCppToolsApi(version: Version): Promise<CppToolsApi | undefined>;
