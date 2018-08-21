'use strict';

const Promise = require('bluebird'),
    _ = require('lodash'),
    mongo = require('models/mongo');

console.log(`ENV : ${process.env.NODE_ENV}`);

if (process.env.NODE_ENV === 'production') {
    console.log('NOT SUPPORT IN PRODUCTION');
    return;
}

const WpsInfo = mongo.WpsInfo;
const Summary = mongo.LibSummary;

// sample
const wpsInfo = {
    watchman_account : 'committeeaab',
    watchman_prv : '5K6LU8aVpBq9vJsnpCvaHCcyYwzPPKXfDdyefYyAMMs3Qy42fUr',
    watchman_pub : 'EOS7WnhaKwHpbSidYuh2DF1qAExTRUtPEdZCaZqt75cKcixuQUtdA',
    total_voting_percent : 5,
    agree_percent : 10
};

async function upsertWps() {
    const wps = await WpsInfo.findOne();
    if (_.isEmpty(wps)) {
        return WpsInfo.create(wpsInfo);
    } else {
        return WpsInfo.updateOne({_id : wps._id}, wpsInfo);
    }
}

async function createSummary() {
    const summary = await Summary.findOne();
    if (_.isEmpty(summary)) {
        return Summary.create({block_num : 100});
    } else {
        // return Summary.updateOne({block_num : 0});
    }
}

Promise.resolve(upsertWps()).delay(100)
    .then(() => {
        return createSummary();
    })
    .then(() => {
        console.log('complete~~');
    })
    .delay(1000);


