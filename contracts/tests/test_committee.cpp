#include "test.hpp"

class committee_tester : public tester {
public:
   committee_tester() {
    create_accounts( {N(eosio.token), N(eosio.wps), N(eosio.saving), N(committee1), N(committee2)} );
    produce_block();

    base_tester::push_action(config::system_account_name, N(setpriv), config::system_account_name,
      mutable_variant_object()
      ("account", "eosio.token")
      ("is_priv", 1)
    );

    base_tester::push_action(config::system_account_name, N(setpriv), config::system_account_name,
      mutable_variant_object()
      ("account", "eosio.wps")
      ("is_priv", 1)
    );

    base_tester::push_action(config::system_account_name, N(setpriv), config::system_account_name,
      mutable_variant_object()
      ("account", "eosio.saving")
      ("is_priv", 1)
    );

    set_code( N(eosio.token), contracts::util::token_wasm() );
    set_abi( N(eosio.token), contracts::util::token_abi().data() );

    set_code( N(eosio.wps), contracts::wps_wasm() );
    set_abi( N(eosio.wps), contracts::wps_abi().data() );

    produce_blocks();

    const auto& accnt = control->db().get<account_object,by_name>( N(eosio.wps) );
    abi_def abi;
    BOOST_REQUIRE_EQUAL(abi_serializer::to_abi(accnt.abi, abi), true);
    abi_ser.set_abi(abi, fc::seconds(1));
   }

  bool get_committee(committee_t& committeeman, account_name name) {
    return get_table_entry(committeeman, N(eosio.wps), N(eosio.wps), N(committees), name);
  }

  bool get_wpsenv(wps_env_t& wpsenv ) {
    return get_table_entry(wpsenv, N(eosio.wps), N(eosio.wps), N(wpsglobal), N(wpsglobal));
  }

  abi_serializer abi_ser;
};

BOOST_AUTO_TEST_SUITE(committee_tests)

BOOST_FIXTURE_TEST_CASE( manage_committee, committee_tester ) try {
  auto committeeman1_1 = mvo()
      ("committeeman",  "committee1")
      ("category", "emergency")
      ("is_oversight", 1);

  auto committeeman2_1 = mvo()
      ("committeeman",  "committee2")
      ("category", "game")
      ("is_oversight", 0);

  auto committeeman1_2 = mvo()
      ("committeeman",  "committee1")
      ("category", "emergency2")
      ("is_oversight", 1);

  auto committeeman2_2 = mvo()
      ("committeeman",  "committee2")
      ("category", "game2")
      ("is_oversight", 0);

  auto wpsenv = mvo()
      ("total_voting_percent",  10)
      ("duration_of_voting", 30)
      ("max_duration_of_funding", 180)
      ("total_iteration_of_funding", 6)
      ("proposal_current_index", 0);

  auto trace = base_tester::push_action( N(eosio.wps), N(setwpsenv), N(eosio.wps), wpsenv);

  produce_block();

  wps_env_t _wpsenv;
  get_wpsenv(_wpsenv);
  REQUIRE_MATCHING_OBJECT(_wpsenv, wpsenv);

  trace = base_tester::push_action( N(eosio.wps), N(regcommittee), N(eosio.wps), committeeman1_1);
  trace = base_tester::push_action( N(eosio.wps), N(regcommittee), N(eosio.wps), committeeman2_1);

  produce_block();

  committee_t t1_1;
  committee_t t2_1;
  get_committee(t1_1, N(committee1));
  get_committee(t2_1, N(committee2));

  REQUIRE_MATCHING_OBJECT(t1_1, committeeman1_1);
  REQUIRE_MATCHING_OBJECT(t2_1, committeeman2_1);

  trace = base_tester::push_action( N(eosio.wps), N(editcommittee), N(eosio.wps), committeeman1_2);
  trace = base_tester::push_action( N(eosio.wps), N(editcommittee), N(eosio.wps), committeeman2_2);

  produce_block();

  committee_t t1_2;
  committee_t t2_2;
  get_committee(t1_2, N(committee1));
  get_committee(t2_2, N(committee2));

  REQUIRE_MATCHING_OBJECT(t1_2, committeeman1_2);
  REQUIRE_MATCHING_OBJECT(t2_2, committeeman2_2);

  trace = base_tester::push_action( N(eosio.wps), N(rmvcommittee), N(eosio.wps), mvo()("committeeman", "committee2"));
  produce_block();

  /*
  try {
    get_committee(t2_2, N(committee2));
  } catch(...) {
    BOOST_TEST_MESSAGE("exception");
  }
  */

  // BOOST_TEST_MESSAGE( fc::json::to_pretty_string(trace) );
  // BOOST_TEST_MESSAGE( fc::json::to_pretty_string(t2_2) );

} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()
