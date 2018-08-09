#include "test.hpp"

class proposal_tester : public tester {
public:
  proposal_tester() {
    create_accounts( {N(eosio.token), N(eosio.wps), N(eosio.saving), N(committee1), N(committee2), N(proposer1), N(proposer2)} );

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

    init_committee();
    init_proposer();
   }

  void init_committee() {
    auto committeeman1 = mvo()
      ("committeeman",  "committee1")
      ("category", "emergency")
      ("is_oversight", 1);

    auto committeeman2 = mvo()
      ("committeeman",  "committee2")
      ("category", "game")
      ("is_oversight", 0);

    auto wpsenv = mvo()
      ("total_voting_percent",  10)
      ("duration_of_voting", 30)
      ("max_duration_of_funding", 180)
      ("total_iteration_of_funding", 6)
      ("proposal_current_index", 0);

    base_tester::push_action( N(eosio.wps), N(setwpsenv), N(eosio.wps), wpsenv);
    produce_block();

    base_tester::push_action( N(eosio.wps), N(regcommittee), N(eosio.wps), committeeman1);
    base_tester::push_action( N(eosio.wps), N(regcommittee), N(eosio.wps), committeeman2);

    produce_block();
  }

  void init_proposer() {
    auto proposer1 = mvo()
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

    auto proposer2 = mvo()
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

    base_tester::push_action( N(eosio.wps), N(regproposer), N(proposer1), proposer1);
    base_tester::push_action( N(eosio.wps), N(regproposer), N(proposer2), proposer2);

    produce_block();
  }

  bool get_proposal(proposal_t& proposal, account_name name) {
    return get_table_entry(proposal, N(eosio.wps), N(eosio.wps), N(proposals), name);
  }

/*
  bool from_variant(const fc::variant_object& o, proposal_t& proposal) {
    fc::raw::unpack(o.value.data(), o.value.size(), proposal);
  }
*/

  void check_compare(const fc::variant_object& proposal, proposal_t& proposal_from_block) {
    auto value = fc::variant(proposal_from_block).get_object();
    for( auto& entry : proposal ) {
      auto it = value.find(entry.key());
      if (entry.key() != "financial_loadmap") {
        BOOST_TEST_MESSAGE(entry.key());
        BOOST_REQUIRE_EQUAL(entry.value(), it->value());
      } else {
        // BOOST_REQUIRE_EQUAL(entry.value(), it->value());
		    // REQUIRE_MATCHING_OBJECT(entry.value(), it->value());
      }
    }
  }

  abi_serializer abi_ser;
};

BOOST_AUTO_TEST_SUITE(proposal_tests)

BOOST_FIXTURE_TEST_CASE( manage_proposal, proposal_tester ) try {
  const std::vector<string>& members{"yepp4you", "yepp4you2", "yepp4you3"};
  auto proposal1_1 = mvo()
    ("proposer", "proposer1")
    ("id", 1)
    ("committee", "committee1")
    ("category", "emergency")
    ("subcategory", 1)
    ("title", "wps project title")
    ("summary", "wps proejct summary")
    ("project_img_url", "http://www.google.com")
    ("description", "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz")
    ("roadmap", "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz")
    ("duration", 30)
    ("members", members)
    ("funding_goal", core_from_string("10.0000"))
    ("total_votes", 0)
    ("status", 1)
    ("vote_start_time", 0)
    ("fund_start_time", 0)
    ("iteration_of_funding", 0);

  auto proposal2_1 = mvo()
    ("proposer", "proposer2")
    ("id", 2)
    ("committee", "committee2")
    ("category", "game")
    ("subcategory", 1)
    ("title", "game title")
    ("summary", "game proejct subtitle")
    ("project_img_url", "http://www.google.com")
    ("description", "game yyyyyyyyyy")
    ("roadmap", "game yyyyyyyyyy")
    ("duration", 30)
    ("members", members)
    ("funding_goal", core_from_string("10.0000"))
    ("total_votes", 0)
    ("status", 1)
    ("vote_start_time", 0)
    ("fund_start_time", 0)
    ("iteration_of_funding", 0);


  auto proposal1_2 = mvo()
    ("proposer", "proposer1")
    ("id", 1)
    ("committee", "committee1")
    ("category", "emergency")
    ("subcategory", 1)
    ("title", "wps project title2")
    ("summary", "game proejct subtitle")
    ("project_img_url", "http://www.google.com")
    ("description", "game yyyyyyyyyy")
    ("roadmap", "game yyyyyyyyyy")
    ("duration", 30)
    ("members", members)
    ("funding_goal", core_from_string("10.0000"))
    ("total_votes", 0)
    ("status", 1)
    ("vote_start_time", 0)
    ("fund_start_time", 0)
    ("iteration_of_funding", 0);

  auto proposal2_2 = mvo()
    ("proposer", "proposer2")
    ("id", 2)
    ("committee", "committee2")
    ("category", "game")
    ("subcategory", 1)
    ("title", "game title2")
    ("summary", "game proejct subtitle")
    ("project_img_url", "http://www.google.com")
    ("description", "game yyyyyyyyyy")
    ("roadmap", "game yyyyyyyyyy")
    ("duration", 30)
    ("members", members)
    ("funding_goal", core_from_string("10.0000"))
    ("total_votes", 0)
    ("status", 1)
    ("vote_start_time", 0)
    ("fund_start_time", 0)
    ("iteration_of_funding", 0);

  // BOOST_TEST_MESSAGE( fc::json::to_pretty_string(proposal1_1) );

  base_tester::push_action( N(eosio.wps), N(regproposal), N(proposer1), proposal1_1);
  base_tester::push_action( N(eosio.wps), N(regproposal), N(proposer2), proposal2_1);

  produce_block();

  proposal_t _p1_1;
  proposal_t _p2_1;
  get_proposal(_p1_1, N(proposer1));
  get_proposal(_p2_1, N(proposer2));

  // BOOST_TEST_MESSAGE( fc::json::to_pretty_string(_p1_1) );
  // BOOST_TEST_MESSAGE( fc::json::to_pretty_string(_p2_1) );

  // REQUIRE_MATCHING_OBJECT(_p1_1, proposal1_1);
  // REQUIRE_MATCHING_OBJECT(_p2_1, proposal2_1);

  // check_compare(proposal1_1, _p1_1);

  base_tester::push_action( N(eosio.wps), N(editproposal), N(proposer1), proposal1_2);
  base_tester::push_action( N(eosio.wps), N(editproposal), N(proposer2), proposal2_2);

  produce_block();

  proposal_t _p1_2;
  proposal_t _p2_2;
  get_proposal(_p1_2, N(proposer1));
  get_proposal(_p2_2, N(proposer2));

  // BOOST_TEST_MESSAGE( fc::json::to_pretty_string(_p1_2) );
  // BOOST_TEST_MESSAGE( fc::json::to_pretty_string(_p2_2) );

  // REQUIRE_MATCHING_OBJECT(_p1_2, proposal1_2);
  // REQUIRE_MATCHING_OBJECT(_p2_2, proposal2_2);
  base_tester::push_action( N(eosio.wps), N(rmvproposal), N(proposer2), mvo()("proposer", "proposer2"));
  produce_block();

} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()
