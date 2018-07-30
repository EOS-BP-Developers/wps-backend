#pragma once
#include <eosio/testing/tester.hpp>

namespace eosio { namespace testing {

struct contracts {
   static std::vector<uint8_t> wps_wasm() { return read_wasm("/Users/yepp4you/Work/blockchain/wps-backend/contracts/eosio.wps/bin/eosio.wps/eosio.wps.wasm"); }
   static std::string          wps_wast() { return read_wast("/Users/yepp4you/Work/blockchain/wps-backend/contracts/eosio.wps/bin/eosio.wps/eosio.wps.wast"); }
   static std::vector<char>    wps_abi() { return read_abi("/Users/yepp4you/Work/blockchain/wps-backend/contracts/eosio.wps/bin/eosio.wps/eosio.wps.abi"); }

   struct util {
      static std::vector<uint8_t> test_api_wasm() { return read_wasm("/Users/yepp4you/Work/blockchain/wps-backend/contracts/tests/test_contracts/test_api.wasm"); }
      static std::vector<uint8_t> exchange_wasm() { return read_wasm("/Users/yepp4you/Work/blockchain/wps-backend/contracts/tests/test_contracts/exchange.wasm"); }

      static std::vector<uint8_t> token_wasm() { return read_wasm("/Users/yepp4you/Work/blockchain/wps-backend/contracts/tests/test_contracts/eosio.token.wasm"); }
      static std::vector<char> token_abi() { return read_abi("/Users/yepp4you/Work/blockchain/wps-backend/contracts/tests/test_contracts/eosio.token.abi"); }
   };
};
}} //ns eosio::testing
