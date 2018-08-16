'use strict';

let Promise = require('bluebird'),
    redis = require('redis'),
    _ = require('lodash'),
    HashRing = require('hashring'),
    redisConfig = require('config').redis;

function RedisLoadBalance(redis, arrHost, defaultPort) {
    if (!(this instanceof RedisLoadBalance)) {
        return new RedisLoadBalance(arrHost);
    }
    let self = this;
    self.mapClient = {};
    if (_.isArray(arrHost) === true) {
        self.hashRing = new HashRing(arrHost);
        _.forEach(arrHost, function(host) {
            let arrHostInfo = host.split(':'),
                port = arrHostInfo.length > 1 ? arrHostInfo[1] : defaultPort;
            self.mapClient[host] = Promise.promisifyAll(redis.createClient(port, arrHostInfo[0]));
        });
    } else {
        self.mapClient[arrHost] = Promise.promisifyAll(redis.createClient(defaultPort, arrHost));
    }
    self.hosts = arrHost;
}

RedisLoadBalance.prototype.getClient = function(key) {
    let host = '';
    if (_.isArray(this.hosts) === true) {
        host = this.hashRing.get(key);
    } else {
        host = this.hosts;
    }
    return this.mapClient[host];
};

RedisLoadBalance.prototype.getClients = function() {
    return this.mapClient;
};

function RedisClient(type, keyIds, version) {
    if (!(this instanceof RedisClient)) {
        return new RedisClient(type, keyIds);
    }
    this.type = type;
    this.keyIds = keyIds || [0];
    this.value = '';
    this.client = RedisClient.getClient(RedisClient.key(type, keyIds));
    this.version = version;
}

RedisClient.initLoadBalance = function(redis, hosts, defaultPort) {
    RedisClient.loadBalance = new RedisLoadBalance(redis, hosts, defaultPort);
};

RedisClient.getClient = function(key) {
    return RedisClient.loadBalance.getClient(key);
};

RedisClient.getClients = function() {
    return RedisClient.loadBalance.getClients();
};

RedisClient.key = function(type, keyIds, version) {
};

RedisClient.expire = function(type) {
};

RedisClient.remove = function(type, keyIds, version, callback) {
    keyIds = keyIds || [];
    let key = RedisClient.key(type, keyIds, version),
        client =  RedisClient.getClient(key);
    client.del(key, function(err, res) {
        if (err) {
            callback(err, res);
            return;
        }
        callback(err, true);
    });
};

RedisClient.removeByPatternPro = function(pattern) {
    let clients = RedisClient.getClients();
    let arrRedis = _.map(clients, (redis, key) => redis);
    return Promise.each(arrRedis, (redis) => {
        return redis.keysAsync(pattern)
            .then((keys) => {
                if (_.isEmpty(keys)) {
                    return null;
                }
                return redis.delAsync(keys);
            });
    });
};

RedisClient.genScore = function genRedisScore(anchorId, size, direction) {
    let score = parseInt(anchorId) * 4000 + parseInt(size);
    if (direction === 'past') {
        score = score * (-1);
    }
    return  score;
};


RedisClient.prototype.remove = function(callback) {
    let key = RedisClient.key(this.type, this.keyIds, this.version);
    this.client.del(key, function(err, res) {
        if (err) {
            callback(err, res);
            return;
        }
        callback(err, true);
    });
};

RedisClient.prototype.save = function(callback) {
    let self = this,
        key = RedisClient.key(self.type, self.keyIds, self.version);
    self.client.set(key, this.value,
        function(err, res) {
            if (err) {
                callback(err, res);
                return;
            }
            let result = true,
                expireTime = RedisClient.expire(self.type);
            if (res !== 'OK') {
                result = false;
            }
            if (!_.isNil(expireTime)) {
                self.client.expire(key, expireTime, function(err, res) {
                    callback(err, result);
                });
            } else {
                callback(err, result);
            }
        });
};

RedisClient.prototype.restore = function(callback) {
    let self = this,
        key = RedisClient.key(self.type, self.keyIds, self.version);
    self.client.get(key, function(err, value) {
        if (err) {
            callback(err, value);
            return;
        }
        self.value = value;
        callback(err, self.value);
    });
};

RedisClient.prototype.saveSorted = function(score, callback) {
    let self = this,
        key = RedisClient.key(self.type, self.keyIds, self.version);
    this.client.zadd(key, score, this.value,
        function(err, res) {
            if (err) {
                callback(err, res);
                return;
            }
            let result = true,
                expireTime = RedisClient.expire(self.type);
            if (res !== 'OK') {
                result = false;
            }
            if (!_.isNil(expireTime)) {
                self.client.expire(key, expireTime, function(err, res) {
                    callback(err, result);
                });
            } else {
                callback(err, result);
            }
        });
};

RedisClient.prototype.restoreSorted = function(score, callback) {
    let self = this,
        key = RedisClient.key(self.type, self.keyIds, self.version),
        start = score,
        stop = '(' + (score + 1);
    self.client.zrangebyscore(key, start, stop, function(err, value) {
        if (err) {
            callback(err, value);
            return;
        }
        self.value = value;
        callback(err, self.value);
    });
};

RedisClient.prototype.restoreSortedRange = function(start, stop, callback) {
    let self = this,
        key = RedisClient.key(self.type, self.keyIds, self.version);
    self.client.zrangebyscore(key, start, stop, function(err, value) {
        if (err) {
            callback(err, value);
            return;
        }
        self.value = value;
        callback(err, self.value);
    });
};

RedisClient.prototype.restoreSortedMaxScore = function(callback) {
    let self = this,
        key = RedisClient.key(self.type, self.keyIds, self.version);
    let args = [ key, '+inf', '-inf', 'WITHSCORES', 'LIMIT', 0, 1];
    self.client.zrevrangebyscore(args, function(err, value) {
        if (err) {
            callback(err, value);
            return;
        }
        self.value = value;
        callback(err, self.value);
    });
};

RedisClient.initLoadBalance(redis, redisConfig.hosts, redisConfig.port);

Promise.promisifyAll(RedisClient);
Promise.promisifyAll(RedisClient.prototype);

module.exports = RedisClient;
