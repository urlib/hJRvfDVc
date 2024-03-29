///<reference path="..\typings\globals\node\index.d.ts" />
"use strict";
var fs = require("fs");
var http = require("http");
var https = require("https");
var os = require("os");
var path = require("path");
var url = require("url");
var zlib = require("zlib");
var Logging = require("./Logging");
var AutoCollectClientRequests = require("../AutoCollection/ClientRequests");
var Sender = (function () {
    function Sender(getUrl, onSuccess, onError) {
        this._getUrl = getUrl;
        this._onSuccess = onSuccess;
        this._onError = onError;
        this._enableOfflineMode = false;
        this._resendInterval = Sender.WAIT_BETWEEN_RESEND;
    }
    /**
    * Enable or disable offline mode
    */
    Sender.prototype.setOfflineMode = function (value, resendInterval) {
        this._enableOfflineMode = value;
        if (typeof resendInterval === 'number' && resendInterval >= 0) {
            this._resendInterval = Math.floor(resendInterval);
        }
    };
    Sender.prototype.send = function (payload, callback) {
        var _this = this;
        var endpointUrl = this._getUrl();
        if (endpointUrl && endpointUrl.indexOf("//") === 0) {
            // use https if the config did not specify a protocol
            endpointUrl = "https:" + endpointUrl;
        }
        // todo: investigate specifying an agent here: https://nodejs.org/api/http.html#http_class_http_agent
        var parsedUrl = url.parse(endpointUrl);
        var options = {
            host: parsedUrl.hostname,
            port: parsedUrl.port,
            path: parsedUrl.pathname,
            method: "POST",
            headers: {
                "Content-Type": "application/x-json-stream"
            }
        };
        zlib.gzip(payload, function (err, buffer) {
            var dataToSend = buffer;
            if (err) {
                Logging.warn(err);
                dataToSend = payload; // something went wrong so send without gzip
                options.headers["Content-Length"] = payload.length;
            }
            else {
                options.headers["Content-Encoding"] = "gzip";
                options.headers["Content-Length"] = buffer.length;
            }
            Logging.info(Sender.TAG, options);
            // Ensure this request is not captured by auto-collection.
            options[AutoCollectClientRequests.disableCollectionRequestOption] = true;
            var requestCallback = function (res) {
                res.setEncoding("utf-8");
                //returns empty if the data is accepted
                var responseString = "";
                res.on("data", function (data) {
                    responseString += data;
                });
                res.on("end", function () {
                    Logging.info(Sender.TAG, responseString);
                    if (typeof _this._onSuccess === "function") {
                        _this._onSuccess(responseString);
                    }
                    if (typeof callback === "function") {
                        callback(responseString);
                    }
                    if (_this._enableOfflineMode) {
                        // try to send any cached events if the user is back online
                        if (res.statusCode === 200) {
                            setTimeout(function () { return _this._sendFirstFileOnDisk(); }, _this._resendInterval);
                        }
                        else if (res.statusCode === 206 ||
                            res.statusCode === 429 ||
                            res.statusCode === 439) {
                            _this._storeToDisk(payload);
                        }
                    }
                });
            };
            var req = (parsedUrl.protocol == "https:") ?
                https.request(options, requestCallback) :
                http.request(options, requestCallback);
            req.on("error", function (error) {
                // todo: handle error codes better (group to recoverable/non-recoverable and persist)
                Logging.warn(Sender.TAG, error);
                _this._onErrorHelper(error);
                if (typeof callback === "function") {
                    var errorMessage = "error sending telemetry";
                    if (error && (typeof error.toString === "function")) {
                        errorMessage = error.toString();
                    }
                    callback(errorMessage);
                }
                if (_this._enableOfflineMode) {
                    _this._storeToDisk(payload);
                }
            });
            req.write(dataToSend);
            req.end();
        });
    };
    Sender.prototype.saveOnCrash = function (payload) {
        this._storeToDiskSync(payload);
    };
    Sender.prototype._confirmDirExists = function (direcotry, callback) {
        fs.exists(direcotry, function (exists) {
            if (!exists) {
                fs.mkdir(direcotry, function (err) {
                    callback(err);
                });
            }
            else {
                callback(null);
            }
        });
    };
    /**
     * Stores the payload as a json file on disk in the temp direcotry
     */
    Sender.prototype._storeToDisk = function (payload) {
        var _this = this;
        //ensure directory is created
        var direcotry = path.join(os.tmpdir(), Sender.TEMPDIR);
        this._confirmDirExists(direcotry, function (error) {
            if (error) {
                _this._onErrorHelper(error);
                return;
            }
            //create file - file name for now is the timestamp, a better approach would be a UUID but that
            //would require an external dependency
            var fileName = new Date().getTime() + ".ai.json";
            var fileFullPath = path.join(direcotry, fileName);
            Logging.info(Sender.TAG, "saving data to disk at: " + fileFullPath);
            fs.writeFile(fileFullPath, payload, function (error) { return _this._onErrorHelper(error); });
        });
    };
    /**
     * Stores the payload as a json file on disk using sync file operations
     * this is used when storing data before crashes
     */
    Sender.prototype._storeToDiskSync = function (payload) {
        var direcotry = path.join(os.tmpdir(), Sender.TEMPDIR);
        try {
            if (!fs.existsSync(direcotry)) {
                fs.mkdirSync(direcotry);
            }
            //create file - file name for now is the timestamp, a better approach would be a UUID but that
            //would require an external dependency
            var fileName = new Date().getTime() + ".ai.json";
            var fileFullPath = path.join(direcotry, fileName);
            Logging.info(Sender.TAG, "saving data before crash to disk at: " + fileFullPath);
            fs.writeFileSync(fileFullPath, payload);
        }
        catch (error) {
            this._onErrorHelper(error);
        }
    };
    /**
     * Check for temp telemetry files
     * reads the first file if exist, deletes it and tries to send its load
     */
    Sender.prototype._sendFirstFileOnDisk = function () {
        var _this = this;
        var tempDir = path.join(os.tmpdir(), Sender.TEMPDIR);
        fs.exists(tempDir, function (exists) {
            if (exists) {
                fs.readdir(tempDir, function (error, files) {
                    if (!error) {
                        files = files.filter(function (f) { return path.basename(f).indexOf(".ai.json") > -1; });
                        if (files.length > 0) {
                            var firstFile = files[0];
                            var filePath = path.join(tempDir, firstFile);
                            fs.readFile(filePath, function (error, payload) {
                                if (!error) {
                                    // delete the file first to prevent double sending
                                    fs.unlink(filePath, function (error) {
                                        if (!error) {
                                            _this.send(payload);
                                        }
                                        else {
                                            _this._onErrorHelper(error);
                                        }
                                    });
                                }
                                else {
                                    _this._onErrorHelper(error);
                                }
                            });
                        }
                    }
                    else {
                        _this._onErrorHelper(error);
                    }
                });
            }
        });
    };
    Sender.prototype._onErrorHelper = function (error) {
        if (typeof this._onError === "function") {
            this._onError(error);
        }
    };
    Sender.TAG = "Sender";
    // the amount of time the SDK will wait between resending cached data, this buffer is to avoid any throtelling from the service side
    Sender.WAIT_BETWEEN_RESEND = 60 * 1000;
    Sender.TEMPDIR = "appInsights-node";
    return Sender;
}());
module.exports = Sender;
