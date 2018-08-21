'use strict';

const _ = require('lodash'),
    mongo = require('models/mongo'),
    SUtil = require('libs/util');

const User = mongo.User;

async function createUser(params) {
    return User.create(params);
}

async function updateUser(params) {
    return User.findOneAndUpdate({eos_account : params.eos_account}, {$set : params});
}

async function getUser(eosAccount) {
    return User.findOne({eos_account : eosAccount});
}

async function deleteUser(eosAccount) {
    if (_.isEmpty(eosAccount)) {
        return SUtil.createErrObject('BAD_REQUEST', {reason : 'eos_account is empty'});
    } else if (process.env.NODE_ENV !== 'developer') {
        throw SUtil.createErrObject('BAD_REQUEST', {reason : 'not support'});
    }
    return User.deleteOne({eos_account : eosAccount});
}

module.exports = exports = {createUser, updateUser, getUser, deleteUser};
