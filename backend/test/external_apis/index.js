'use strict';

require('chai').should();
const eosApi = require('external_apis/eos_api');

describe('[external_apis]', function() {
    it('getInfo', async function() {
        return eosApi.getInfo()
        .then(function(result) {
            console.log(result);
        });
    });

    it('getBlock', async function() {
        return eosApi.getBlock(10)
        .then(function(result) {
            console.log(result);
        });
    });

    it('getVoterInfo', async function() {
        return eosApi.getVoterInfo('useraaaaaabg')
        .then(function(result) {
            console.log(result);
        });
    });

    it('getProposalById', async function() {
        return eosApi.getProposalById(1)
        .then(function(result) {
            console.log(result);
        });
    });

    it('getProposalById', async function() {
        return eosApi.getProposalById(2)
        .then(function(result) {
            console.log(result);
        });
    });

    it('getProposalById', async function() {
        return eosApi.getProposalById(3)
        .then(function(result) {
            console.log(result);
        });
    });

    it('getProposalById', async function() {
        return eosApi.getProposalById(4)
        .then(function(result) {
            console.log(result);
        });
    });

    it.skip('getProposalByOwner', async function() {
        return eosApi.getProposalByOwner('proposeraaaa')
        .then(function(result) {
            console.log(result);
        });
    });

    it('getRejectedProposalById', async function() {
        return eosApi.getRejectedProposalById(10)
        .then(function(result) {
            console.log(result);
        });
    });

    it('getFinishedProposalById', async function() {
        return eosApi.getFinishedProposalById(11)
        .then(function(result) {
            console.log(result);
        });
    });

    it('getCurrencyStats', async function() {
        return eosApi.getCurrencyStats()
        .then(function(result) {
            console.log(result);
        });
    });

});

