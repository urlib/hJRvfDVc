require('../../modules/es.promise');
require('../../modules/es.promise.finally');
var entryUnbind = require('../../internals/entry-unbind');

module.exports = entryUnbind('Promise', 'finally');
