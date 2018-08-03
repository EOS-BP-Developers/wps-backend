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

struct proposer_t {
  account_name account;
  string first_name;
  string last_name;
  string img_url;
  string bio;
  string country;
  string telegram;
  string website;
  string linkedin;
  uint64_t last_claim_time;
  uint64_t primary_key() const { return account; }
};
FC_REFLECT( proposer_t, (account)(first_name)(last_name)(img_url)(bio)(country)(telegram)(website)(linkedin)(last_claim_time) )

struct proposal_t {
  account_name proposer;        // proposer
  uint64_t id;
  account_name committee;       // committee
  string category;              // category
  uint16_t subcategory;         // subcategory
  string title;                 // title
  string subtitle;              // subtitle
  string project_img_url;       // project image or video url
  string project_overview;      // overview
  string financial_roadmap;     // financial
  vector<string> members;       // linkedin
  asset funding_goal;           // amount of EOS
  uint64_t total_votes;         // total votes
  uint8_t status;               // status
  uint64_t vote_start_time;     // time when voting starts (seconds)
  uint64_t fund_start_time;     // time when funding starts (seconds)
  uint8_t iteration_of_funding; // number of iteration
  uint64_t primary_key() const { return proposer; }
  uint64_t by_id() const { return static_cast<uint64_t>(id); }
};
FC_REFLECT( proposal_t, (proposer)(id)(committee)(category)(subcategory)(title)(subtitle)(project_img_url)(project_overview)(financial_roadmap)(members)(funding_goal)(total_votes)(status)(vote_start_time)(fund_start_time)(iteration_of_funding) )
