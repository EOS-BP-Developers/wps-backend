'use strict';

const mongo = require('models/mongo');

const Committee = mongo.LibCommittee;

function getCommittees() {
    return Committee.find().sort({_id : 1});
}

module.exports = exports = {getCommittees};
