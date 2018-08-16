'use strict';

const mongo = require('models/mongo');

const Summary = mongo.LibSummary;

function getSummary() {
    return Summary.findOne();
}

module.exports = exports = {getSummary};
