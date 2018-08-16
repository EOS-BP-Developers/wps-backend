#!/bin/bash
################################################################################
#
# Scrip Created by http://CryptoLions.io
# For EOS mainnet
#
# https://github.com/CryptoLions/EOS-MainNet
#
###############################################################################

NODEHOST="127.0.0.1"
NODEPORT="8888"

WALLETHOST="127.0.0.1"
WALLETPORT="8888"

echo NODE HOST : $NODEHOST:$NODEPORT

cleos -u http://$NODEHOST:$NODEPORT --wallet-url http://$WALLETHOST:$WALLETPORT "$@"
