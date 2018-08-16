'use strict';

const _ = require('lodash'),
    SEnum = require('libs/enum'),
    RedisClient = require('./redis_client'),
    config = require('config').redis;

RedisClient.key = function(type, keyIds, version) {
    let keyStr = _key(type, keyIds);
    if (!_.isNil(version)) {
        keyStr = `${keyStr}:v:${version}`;
    }
    return keyStr;
};

RedisClient.expire = function(type) {
    var expireTime = config.expire.default;
    switch (type) {
        // auth
        case SEnum.REDIS_TYPE_PRODUCER_SESSION:
            expireTime = config.expire.redis_admin_session;
            break;
    }
    return expireTime || config.expire.default;
};

function _key(type, keyIds) {
    switch (type) {
        case SEnum.REDIS_TYPE_BATCH_STATUS:
            return `eos:batch:status:${keyIds[0]}`;  // job name
        case SEnum.REDIS_TYPE_BATCH_CMD:
            return `eos:batch:cmd:${keyIds[0]}`;     // job name
        case SEnum.REDIS_TYPE_BATCH_CMD_ALL:
            return 'eos:batch:cmd:all';

        case SEnum.REDIS_TYPE_FRONTEND_MANIFEST:   // front-end
            return 'm:frontend:manifest';

    }
    return 'none';
}

module.exports = RedisClient;
