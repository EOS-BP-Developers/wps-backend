'use strict';

let winston = require('winston'),
    moment = require('moment'),
    DailyRotateFile = require('winston-daily-rotate-file'),
    config = require('config');

let timezone = config.timezone;

let timestamp = function() {
    return moment().utcOffset(timezone).format();
};

config.webLog.timestamp = timestamp;
config.dbLog.timestamp = timestamp;
config.errLog.timestamp = timestamp;
config.batchLog.timestamp = timestamp;
config.batchErrLog.timestamp = timestamp;

let webLogStream = {
    write: function(message, encoding) {
        webLog.info(message);
    }
};

let webLog = new (winston.Logger)({
    transports: [
        //new (winston.transports.Console)({level : config.log.level}),
        new DailyRotateFile(config.webLog)
    ]
});

let dbLog = new (winston.Logger)({
    transports: [
        new DailyRotateFile(config.dbLog)
    ]
});

let errLog = new (winston.Logger)({
    transports: [
        new DailyRotateFile(config.errLog)
    ]
});

let batchLog = new (winston.Logger)({
    transports: [
        new DailyRotateFile(config.batchLog)
    ]
});

let batchErrLog = new (winston.Logger)({
    transports: [
        new DailyRotateFile(config.batchErrLog)
    ]
});

config.dbLog = dbLog;

module.exports = {
    webLogStream : webLogStream,
    webLog : webLog,
    dbLog : dbLog,
    errLog : errLog,
    batchLog : batchLog,
    batchErrLog : batchErrLog
};
