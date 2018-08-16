'use strict';

const express = require('express'),
    log = require('libs/log'),
    Proposal = require('libs/api/proposal'),
    SUtil = require('libs/util'),
    SErr = require('libs/error');

const errLog = log.errLog;
const router = express.Router();

let getProposals = function(req, res, next) {
    Proposal.getProposals(req.params)
        .then(function(proposals) {
            res.json({proposals});
        })
        .catch(function(err) {
            errorRes(res, err);
        });

};

let getProposal = function(req, res, next) {
    const proposalId = parseInt(req.params.proposal_id);
    Proposal.getProposal(proposalId)
        .then(function(proposal) {
            res.json({proposal});
        })
        .catch(function(err) {
            errorRes(res, err);
        });
};


function errorRes(res, err) {
    errLog.info(err);
    errLog.info(err.stack);

    if (err.code === SErr.BAD_REQUEST.error_code) {
        res.json(SUtil.createErrResult('BAD_REQUEST'));
    } else if (err.code === SErr.NOT_ACCEPTABLE.error_code) {
        res.json(SUtil.createErrResult('NOT_ACCEPTABLE'));
    } else if (err.code === SErr.DUPLICATE_ERROR.error_code) {
        res.json(SUtil.createErrResult('DUPLICATE_ERROR'));
    } else if (err.code === SErr.NODE_INVALIDATE_ERROR.error_code) {
        res.json(SUtil.createErrResult('NODE_INVALIDATE_ERROR'));
    }
    else {
        res.json(SUtil.createErrResult('INTERNAL_SERVER_ERROR', err.error));
    }
}

router.get('/1/committees/:committee/proposals/orders/:order', getProposals);
router.get('/1/committees/:committee/proposals/:proposal_id', getProposal);

module.exports = exports = router;
