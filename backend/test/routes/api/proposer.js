'use strict';

let Promise = require('bluebird'),
    chai = require('chai'),
    app = require('app'),
    data = require('test/data');

let request = Promise.promisifyAll(require('supertest'));
chai.should();

/*
router.get('/1/proposers/:proposer/proposals', getProposals);
*/

module.exports = exports = function() {
    const proposers = data.proposers;
    const proposer = proposers[0];
    it('get /1/proposers/:proposer/proposals should receive proposals', function() {
        return request(app)
            .get(`/api/1/proposers/${proposer.account}/proposals`)
            .endAsync().then(function(res) {
                res.body.should.not.have.property('error');
                res.body.should.have.property('proposals');
                console.log(res.body.proposals);
            });
    });
};
