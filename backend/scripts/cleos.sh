#!/bin/bash
################################################################################
#
# Scrip Created by http://CryptoLions.io
# For EOS mainnet
#
# https://github.com/CryptoLions/EOS-MainNet
#
###############################################################################

NODEHOST="user-api.eoseoul.io"
NODEPORT="80"

WALLETHOST="127.0.0.1"
WALLETPORT="8900"

echo NODE HOST : $NODEHOST:$NODEPORT

cleos -u http://$NODEHOST:$NODEPORT --wallet-url http://$WALLETHOST:$WALLETPORT "$@"
