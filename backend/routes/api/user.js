'use strict';

const express = require('express'),
    log = require('libs/log'),
    User = require('libs/api/user'),
    SUtil = require('libs/util'),
    SErr = require('libs/error');

const errLog = log.errLog;
const router = express.Router();

let createUser = function(req, res, next) {
    User.createUser(req.body)
        .then(function(user) {
            res.json({user});
        })
        .catch(function(err) {
            errorRes(res, err);
        });
};

let updateUser = function(req, res, next) {
    User.updateUser(req.body)
        .then(function(user) {
            res.json({user});
        })
        .catch(function(err) {
            errorRes(res, err);
        });
};

let getUser = function(req, res, next) {
    User.getUser(req.params.eos_account)
        .then(function(user) {
            res.json({user});
        })
        .catch(function(err) {
            errorRes(res, err);
        });
};

let deleteUser = function(req, res, next) {
    User.deleteUser(req.params.eos_account)
        .then(function(result) {
            res.json({result : 'OK', eos_account : req.params.eos_account});
        })
        .catch(function(err) {
            errorRes(res, err);
        });
};

function errorRes(res, err) {
    errLog.info(JSON.stringify(err));
    errLog.info(err.stack);

    if (err.code === SErr.BAD_REQUEST.error_code) {
        res.json(SUtil.createErrResult('BAD_REQUEST'));
    } else if (err.code === SErr.NOT_ACCEPTABLE.error_code) {
        res.json(SUtil.createErrResult('NOT_ACCEPTABLE'));
    } else if (err.code === SErr.DUPLICATE_ERROR.error_code) {
        res.json(SUtil.createErrResult('DUPLICATE_ERROR'));
    }
    else {
        res.json(SUtil.createErrResult('INTERNAL_SERVER_ERROR', err.error));
    }
}

router.post('/1/users', createUser);
router.put('/1/users/:eos_account', updateUser);
router.get('/1/users/:eos_account', getUser);
router.delete('/1/users/:eos_account', deleteUser);

module.exports = exports = router;
