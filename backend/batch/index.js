'use strict';

let _ = require('lodash'),
    batchConfig = require('batch/batch_config');

let jobGroup = process.env.NODE_JOB_GROUP,
    arrJob = [];

if (!_.isEmpty(jobGroup)) {
    let arrJobGroup = jobGroup.split('|');
    _.forEach(arrJobGroup, (group) => {
        group = _.trim(group);
        let batch = _.find(batchConfig, {job_group: group});
        if (!_.isEmpty(batch)) {
            arrJob = _.union(arrJob, batch.jobs);
        }
    });

    arrJob = _.map(arrJob, function(job) {
        if (_.indexOf(job.excludes, process.env.NODE_ENV) >= 0) {
            console.log('exclude :' + job.name);
            return;
        }
        return job;
    });
    arrJob = _.compact(arrJob);
}

if (_.isEmpty(arrJob)) {
    module.exports = null;
    return ;
}

let Promise = require('bluebird'),
    schedule = require('node-schedule'),
    moment = require('moment'),
    RedisClient = require('models/redis'),
    log = require('libs/log'),
    SEnum = require('libs/enum');

const errLog = log.errLog;

let isRegistered = false,
    arrBatchJob = [];

function wrapFunc(funcPro, params, jobName) {
    let isRunning = false;
    return function() {
        let startedAt = moment.utc();
        if (!isRunning) {
            let batchCmd = new RedisClient(SEnum.REDIS_TYPE_BATCH_CMD, [jobName]);
            let batchCmdAll = new RedisClient(SEnum.REDIS_TYPE_BATCH_CMD_ALL, []);
            let batchStatus = new RedisClient(SEnum.REDIS_TYPE_BATCH_STATUS, [jobName]);

            return Promise.join(
                batchCmd.restoreAsync(),
                batchCmdAll.restoreAsync(),
                function(cmd, cmdAll) {
                    if (cmd == 'PAUSE' || cmdAll === 'PAUSE') {
                        throw new Error('PAUSE');
                    }
                    isRunning = true;
                    batchStatus.value = JSON.stringify({status : 'RUN', date : startedAt.format('YYYY-MM-DD HH:mm:ssZ')});
                    return batchStatus.saveAsync();
                })
                .then(function() {
                    return funcPro(params);
                })
                .then(function(result) {
                    let endedAt = moment.utc();
                    isRunning = false;

                    batchStatus.value = JSON.stringify({status : 'END', date : endedAt.format('YYYY-MM-DD HH:mm:ssZ')});
                    return batchStatus.saveAsync();
                })
                .catch(function(err) {
                    isRunning = false;
                    if (err.message == 'PAUSE') {
                        batchStatus.value = JSON.stringify({status : 'PAUSE'});
                        return batchStatus.saveAsync();
                    } else {
                        errLog.info(err);
                        errLog.info(err.stack);
                        /*
                        let batchJob = _.find(arrBatchJob, {name : jobName});
                        if (!_.isEmpty(batchJob)) {
                            batchJob.job.cancel();
                            isRunning = false;
                        }
                        */
                    }
                });
        }
    };
}

let registerJobs = function() {
    if (isRegistered === true) {
        return isRegistered;
    }
    const processActions = require('libs/batch/process_actions');
    const updateProposals = require('libs/batch/update_proposals');
    const updateVotings = require('libs/batch/update_votings');
    const updateSummary = require('libs/batch/update_summary');
    let funcMap = _.extend(processActions, updateProposals, updateVotings, updateSummary);
    return Promise.map(arrJob, function(job) {
        let rule = new schedule.RecurrenceRule(),
            jobName = job.name,
            scheduleInfo = job.schedule;

        if (!_.isNil(scheduleInfo.hour)) {
            rule.hour = scheduleInfo.hour;
        }
        if (!_.isNil(scheduleInfo.minute)) {
            rule.minute = scheduleInfo.minute;
        }
        if (!_.isNil(scheduleInfo.second)) {
            rule.second = scheduleInfo.second;
        }

        if (!_.isEmpty(jobName)) {
            let func = wrapFunc(funcMap[jobName], job.params, jobName),
                j = null;

            if (!_.isNil(func)) {
                let batchCmd = new RedisClient(SEnum.REDIS_TYPE_BATCH_CMD, [jobName]);
                let batchCmdAll = new RedisClient(SEnum.REDIS_TYPE_BATCH_CMD_ALL, []);

                j = schedule.scheduleJob(rule, func);
                arrBatchJob.push({job : j, name : jobName});

                batchCmd.value = 'START';
                batchCmdAll.value = 'START';
                return Promise.join(
                    batchCmd.saveAsync(),
                    batchCmdAll.saveAsync(),
                    function(result1, result2) {

                    });
            }
        }
    })
    .then(function() {
        isRegistered = true;
        return isRegistered;
    });

};

let unregisterJobs = function() {
    _.forEach(arrBatchJob, (batchJob) => batchJob.job.cancel());
    arrBatchJob = [];
    isRegistered = false;
};

let isRegisteredJobs = function() {
    return isRegistered;
};

module.exports = {
    registerJobs : registerJobs,
    unregisterJobs : unregisterJobs,
    isRegisteredJobs : isRegisteredJobs,
};
