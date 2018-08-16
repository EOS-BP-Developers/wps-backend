'use strict';

const _ = require('lodash');

module.exports = [
    {
        /*
        job_group : 'xxx_log',
        jobs : []
        */
    }, {
        job_group : 'libBlock',
        jobs : [
            {name : 'processActions', params : {}, schedule : {second : _.range(1, 60, 1)}, excludes : ['']},
            {name : 'updateProposals', params : {}, schedule : {second : _.range(1, 60, 1)}, excludes : ['']},
            {name : 'updateVotings', params : {}, schedule : {second : _.range(1, 60, 1)}, excludes : ['']},
            {name : 'updateSummary', params : {}, schedule : {second : _.range(1, 60, 1)}, excludes : ['']},
        ]
    }
];
