'use strict';

let Promise = require('bluebird'),
    chai = require('chai'),
    app = require('app'),
    data = require('test/data');

let request = Promise.promisifyAll(require('supertest'));
chai.should();

/*
router.get('/1/committees/:committee/proposals/orders/:order', getProposals);
router.get('/1/committees/:committee/proposals/:proposal_id', getProposal);
*/

module.exports = exports = function() {
    const committees = data.committees;
    const committee = committees[0];
    it('get /1/committees/:committee/proposals/orders/hot should receive proposals', function() {
        return request(app)
            .get(`/api/1/committees/${committee.committeeman}/proposals/orders/hot`)
            .endAsync().then(function(res) {
                res.body.should.not.have.property('error');
                res.body.should.have.property('proposals');
                console.log(res.body.proposals);
            });
    });

    it('get /1/committees/:committee/proposals/orders/latest should receive proposals', function() {
        return request(app)
            .get(`/api/1/committees/${committee.committeeman}/proposals/orders/latest`)
            .endAsync().then(function(res) {
                res.body.should.not.have.property('error');
                res.body.should.have.property('proposals');
                console.log(res.body.proposals);
            });
    });

    it('get /1/committees/:committee/proposals/:proposal_id should receive proposal', function() {
        return request(app)
            .get(`/api/1/committees/${committee.committeeman}/proposals/1`)
            .endAsync().then(function(res) {
                res.body.should.not.have.property('error');
                res.body.should.have.property('proposal');
                console.log(res.body.proposal);
            });
    });
};
