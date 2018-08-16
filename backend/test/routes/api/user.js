'use strict';

let Promise = require('bluebird'),
    chai = require('chai'),
    app = require('app'),
    data = require('test/data');

let request = Promise.promisifyAll(require('supertest'));
chai.should();

/*
router.post('/1/users', createUser);
router.put('/1/users/:user', updateUser);
router.get('/1/users/:user', getUser);
*/

module.exports = exports = function() {
    const user = data.user;
    let userRes = null;
    it('post /1/users should receive user', function() {
        return request(app)
            .post('/api/1/users')
            .send(user)
            .endAsync().then(function(res) {
                res.body.should.not.have.property('error');
                res.body.should.have.property('user');
                userRes = res.body.user;
            });
    });

    it('put /1/users/:eos_account should receive user', function() {
        return request(app)
            .put(`/api/1/users/${userRes._id}`)
            .send(user)
            .endAsync().then(function(res) {
                res.body.should.not.have.property('error');
                res.body.should.have.property('user');
                userRes = res.body.user;
            });
    });

    it('get /1/users/:eos_account should receive user', function() {
        return request(app)
            .get(`/api/1/users/${userRes.eos_account}`)
            .endAsync().then(function(res) {
                res.body.should.not.have.property('error');
                res.body.should.have.property('user');
            });
    });

    it('delete /1/users/:eos_account should receive user', function() {
        return request(app)
            .delete(`/api/1/users/${userRes.eos_account}`)
            .endAsync().then(function(res) {
                res.body.should.not.have.property('error');
                res.body.should.have.property('result');
            });
    });
};
