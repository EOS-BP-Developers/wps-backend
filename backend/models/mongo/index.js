'use strict';

const mongoose = require('mongoose'),
    _ = require('lodash'),
    config = require('config'),
    log = require('libs/log');

mongoose.Promise = require('bluebird');

mongoose.set('useFindAndModify', false);
mongoose.set('useCreateIndex', true);

const mongoConfig = config.mongo;
const dbLog = log.dbLog;

mongoose.set('debug', function(coll, method, query, doc, options) {
    dbLog.info({dbQuery : {coll, method, query, doc, options}});
});

function makeConnection(dbConfig) {
    var opts = null,
        uri = '';
    if (!_.isEmpty(dbConfig.rsName)) {
        opts = {
            autoReconnect : true,
            poolSize: 30,
            rs_name : dbConfig.rsName,
        };

        uri = makeReplSetUri(dbConfig);
    } else {
        opts = {
            autoReconnect : true,
            poolSize: 30,
            useNewUrlParser: true
        };
        uri = makeUri(dbConfig);
    }
    return mongoose.createConnection(uri, opts);
}

//'mongodb://user:pass@localhost:port,anotherhost:port,yetanother:port/database
function makeReplSetUri(dbConfig) {
    var arrUri = [],
        uri = null;

    uri = `mongodb://${dbConfig.account}:${dbConfig.passwd}@`;
    _.forEach(dbConfig.hosts, (host) => arrUri.push(host));
    uri += arrUri.join(',');
    uri = `${uri}\${dbConfig.dbName}`;
    return uri;
}

//'mongodb://user:pass@localhost:port/database
function makeUri(dbConfig) {
    return `mongodb://${dbConfig.account}:${dbConfig.passwd}@${dbConfig.host}/${dbConfig.dbName}`;
}

function createModel(constructor, conn) {
    return constructor(mongoose, conn);
}

const conn = makeConnection(mongoConfig);

module.exports = exports = {
    User : createModel(require('models/mongo/user'), conn),
    WpsInfo : createModel(require('models/mongo/wps_info'), conn),
    LibAction : createModel(require('models/mongo/lib_action'), conn),
    LibActionStake : createModel(require('models/mongo/lib_action_stake'), conn),
    LibActionUnstake : createModel(require('models/mongo/lib_action_unstake'), conn),

    LibCommittee : createModel(require('models/mongo/lib_committee'), conn),
    LibProposal : createModel(require('models/mongo/lib_proposal'), conn),
    LibProposer : createModel(require('models/mongo/lib_proposer'), conn),
    LibReviewer : createModel(require('models/mongo/lib_reviewer'), conn),
    LibSummary : createModel(require('models/mongo/lib_summary'), conn),
    LibUpdateProposal : createModel(require('models/mongo/lib_update_proposal'), conn),
    LibUpdateVoting : createModel(require('models/mongo/lib_update_voting'), conn),
    LibVoterInfo : createModel(require('models/mongo/lib_voter_info'), conn),
    LibVotingInfo : createModel(require('models/mongo/lib_voting_info'), conn),
};
