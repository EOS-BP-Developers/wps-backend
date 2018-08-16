'use strict';

let Promise = require('bluebird'),
    _ = require('lodash'),
    crypto = require('crypto'),
    request = require('request'),
    config = require('config'),
    SErr = require('libs/error');

let createHash = function(seed, salt) {
    let sha256 = crypto.createHash('sha256');
    sha256.update(seed + salt);
    return sha256.digest('hex');
};

let createAccessToken = function(seed, salt) {
    return createHash(seed, salt + config.salt);
};

let createErrResult = function(errorKey, extra) {
    let errorObj = Object.assign({}, SErr[errorKey]);
    errorObj.extra = extra;
    return {error : errorObj};
};

let createErrObject = function(errorKey, extra) {
    let err = new Promise.OperationalError(),
        errorObj = SErr[errorKey];

    err.code = errorObj.error_code;
    err.message = errorObj.error_msg;
    err.errorKey = errorKey;

    if (!_.isNil(extra)) {
        err.extra = extra;
    }
    return err;
};

function requestPro(options) {
    _.defaults(options, {timeout : 20000});
    return new Promise(function(resolve, reject) {
        request(options, function(err, response, body) {
            if (!_.isEmpty(err)) {
                reject(err);
            } else {
                resolve([response, body]);
            }
        });
    });
}

function requestLoopPro(options, maxLoopCount, timeout, count) {
    count = count || 0;
    return requestPro(options)
    .catch(function(err) {
        if (err.code === 'ETIMEDOUT' || err.code === 'ESOCKETTIMEDOUT') {
            if (count < maxLoopCount) {
                options.timeout = timeout;
                return requestLoopPro(options, maxLoopCount, timeout, ++count);
            }
        }
        throw err;
    });
}

function toCamelCaseKey(map) {
    let ccMap = {};
    _.forEach(map, function(value, key) {
        ccMap[_.camelCase(key)] = value;
    });
    return ccMap;
}

function toAmount(eos) {
    const arr = eos.split(' ');
    return parseInt(arr[0] * 10000, 10);
}

module.exports = {
    createErrResult : createErrResult,
    createErrObject : createErrObject,
    createHash : createHash,
    createAccessToken : createAccessToken,
    requestPro : requestPro,
    requestLoopPro : requestLoopPro,
    toCamelCaseKey : toCamelCaseKey,
    toAmount : toAmount
};

Promise.promisifyAll(module.exports);
