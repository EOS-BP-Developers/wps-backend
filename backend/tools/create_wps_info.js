'use strict';

const Promise = require('bluebird'),
    _ = require('lodash'),
    mongo = require('models/mongo');

const WpsInfo = mongo.WpsInfo;

// sample
const wpsInfo = {
    system_account : 'eosio',
    watchman_account : 'committeeaab',
    watchman_prv : '5K6LU8aVpBq9vJsnpCvaHCcyYwzPPKXfDdyefYyAMMs3Qy42fUr',
    watchman_pub : 'EOS7WnhaKwHpbSidYuh2DF1qAExTRUtPEdZCaZqt75cKcixuQUtdA',
    total_voting_percent : 5,
    agree_percent : 10
};

async function createWps() {
    const wps = await WpsInfo.findOne();
    if (_.isEmpty(wps)) {
        return WpsInfo.create(wpsInfo);
    } else {
        return WpsInfo.update({_id : wps._id}, wpsInfo);
    }
}

Promise.resolve(createWps()).delay(1000)
    .then(() => {
        console.log('complete~~');
    });


