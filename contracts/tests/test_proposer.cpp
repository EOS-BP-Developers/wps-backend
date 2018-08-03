#include "test.hpp"

class proposer_tester : public tester {
public:
  proposer_tester() {
    create_accounts( {N(eosio.token), N(eosio.wps), N(eosio.saving), N(proposer1), N(proposer2)} );

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

  bool get_proposer(proposer_t& proposer, account_name name) {
    return get_table_entry(proposer, N(eosio.wps), N(eosio.wps), N(proposers), name);
  }

  abi_serializer abi_ser;
};

BOOST_AUTO_TEST_SUITE(proposer_tests)

BOOST_FIXTURE_TEST_CASE( manage_proposer, proposer_tester ) try {
  auto proposer1_1 = mvo()
    ("account", "proposer1")
    ("first_name", "Thomas")
    ("last_name","Do")
    ("img_url", "http://www.google.com")
    ("bio", "hi~")
    ("country", "KR")
    ("telegram", "@yepp4you")
    ("website", "http://www.block.one")
    ("linkedin", "thomas-do-01911516a")
    ("last_claim_time", 0);

  auto proposer2_1 = mvo()
    ("account", "proposer2")
    ("first_name", "Thomas")
    ("last_name","Cox")
    ("img_url", "http://www.google.com")
    ("bio", "hi~")
    ("country", "KR")
    ("telegram", "@yepp4you")
    ("website", "http://www.block.one")
    ("linkedin", "thomasbcox")
    ("last_claim_time", 0);

  auto proposer1_2 = mvo()
    ("account", "proposer1")
    ("first_name", "Thomas2")
    ("last_name","Do2")
    ("img_url", "http://www.google.com")
    ("bio", "hi~")
    ("country", "KR")
    ("telegram", "@yepp4you")
    ("website", "http://www.block.one")
    ("linkedin", "thomas-do-01911516a")
    ("last_claim_time", 0);

  auto proposer2_2 = mvo()
    ("account", "proposer2")
    ("first_name", "Thomas2")
    ("last_name","Cox2")
    ("img_url", "http://www.google.com")
    ("bio", "hi~")
    ("country", "KR")
    ("telegram", "@yepp4you")
    ("website", "http://www.block.one")
    ("linkedin", "thomasbcox")
    ("last_claim_time", 0);

  base_tester::push_action( N(eosio.wps), N(regproposer), N(proposer1), proposer1_1);
  base_tester::push_action( N(eosio.wps), N(regproposer), N(proposer2), proposer2_1);

  produce_block();

  proposer_t _c1_1;
  proposer_t _c2_1;
  get_proposer(_c1_1, N(proposer1));
  get_proposer(_c2_1, N(proposer2));

  REQUIRE_MATCHING_OBJECT(_c1_1, proposer1_1);
  REQUIRE_MATCHING_OBJECT(_c2_1, proposer2_1);

  base_tester::push_action( N(eosio.wps), N(editproposer), N(proposer1), proposer1_2);
  base_tester::push_action( N(eosio.wps), N(editproposer), N(proposer2), proposer2_2);

  produce_block();

  proposer_t _c1_2;
  proposer_t _c2_2;
  get_proposer(_c1_2, N(proposer1));
  get_proposer(_c2_2, N(proposer2));

  REQUIRE_MATCHING_OBJECT(_c1_2, proposer1_2);
  REQUIRE_MATCHING_OBJECT(_c2_2, proposer2_2);

  base_tester::push_action( N(eosio.wps), N(rmvproposer), N(proposer2), mvo()("account", "proposer2"));
  produce_block();

} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()
