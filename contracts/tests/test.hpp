#include <boost/test/unit_test.hpp>

#include <eosio/testing/tester.hpp>
#include <eosio/chain/abi_serializer.hpp>
#include <eosio/chain/wast_to_wasm.hpp>

#include <fc/variant_object.hpp>

#include <Runtime/Runtime.h>

#include "contracts.hpp"

using namespace eosio::testing;
using namespace eosio;
using namespace eosio::chain;
using namespace eosio::testing;
using namespace fc;

using mvo = fc::mutable_variant_object;

const uint32_t seconds_per_day = 60 * 60 * 24;

struct __attribute((packed)) committee_t {
  account_name committeeman;
  string category;
  bool is_oversight;
  uint64_t primary_key() const { return committeeman; }
};
FC_REFLECT( committee_t, (committeeman)(category)(is_oversight) );

struct wps_env_t {
  uint64_t proposal_current_index = 0;
  uint32_t total_voting_percent = 5;                      // 5%
  uint32_t duration_of_voting = 30 * seconds_per_day;     // voting duration (seconds)
  uint32_t duration_of_funding = 180 * seconds_per_day;   // funding duration (seconds)
  uint32_t total_iteration_of_funding = 6;                //
};
FC_REFLECT( wps_env_t, (proposal_current_index)(total_voting_percent)(duration_of_voting)(duration_of_funding)(total_iteration_of_funding) );

struct reviewer_t {
    account_name account;
    account_name committee;
    string first_name;
    string last_name;
    uint64_t primary_key() const { return account; }
};
FC_REFLECT( reviewer_t, (account)(committee)(first_name)(last_name) );
