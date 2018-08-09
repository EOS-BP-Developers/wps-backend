#include "test.hpp"

/*
struct PROPOSAL_STATUS_T {
    const static uint8_t PENDING = 1;
    const static uint8_t REJECTED = 2;
    const static uint8_t ON_VOTE = 3;
    const static uint8_t CHECK_VOTE = 4;     // check count of votes
    const static uint8_t CHECKED_VOTE = 5;   // checked count of votes by platform
    const static uint8_t APPROVED = 6;       // approve
    const static uint8_t COMPLETED = 7;
};
*/


class wps_tester : public tester {
public:
  wps_tester() {
    create_accounts( {N(eosio.token), N(eosio.wps), N(eosio.saving), N(committee1), N(committee2), N(watchman1), N(reviewer1), N(reviewer2),  N(proposer1), N(proposer2)});

    create_accounts( {N(voteuser1), N(voteuser2), N(voteuser3), N(voteuser4), N(voteuser5)});

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
    init_reviewer();
    init_proposer();
    init_proposal();
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

    auto watchman1 = mvo()
      ("committeeman",  "watchman1")
      ("category", "game")
      ("is_oversight", 1);

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
    base_tester::push_action( N(eosio.wps), N(regcommittee), N(eosio.wps), watchman1);

    produce_block();
  }

  void init_reviewer() {
    auto reviewer1 = mvo()
      ("reviewer", "reviewer1")
      ("committee", "committee1")
      ("first_name","Thomas")
      ("last_name", "Do");

    auto reviewer2 = mvo()
      ("reviewer", "reviewer2")
      ("committee", "committee2")
      ("first_name","Thomas")
      ("last_name", "Cox");

    base_tester::push_action( N(eosio.wps), N(regreviewer), N(committee1), reviewer1);
    base_tester::push_action( N(eosio.wps), N(regreviewer), N(committee2), reviewer2);
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

  void init_proposal() {
    const std::vector<string>& members{"yepp4you", "yepp4you2", "yepp4you3"};
    auto proposal1 = mvo()
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

    auto proposal2 = mvo()
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

    base_tester::push_action( N(eosio.wps), N(regproposal), N(proposer1), proposal1);
    base_tester::push_action( N(eosio.wps), N(regproposal), N(proposer2), proposal2);
  }

  bool get_proposal(proposal_t& proposal, account_name name) {
    return get_table_entry(proposal, N(eosio.wps), N(eosio.wps), N(proposals), name);
  }

  abi_serializer abi_ser;
};

BOOST_AUTO_TEST_SUITE(wps_tests)

BOOST_FIXTURE_TEST_CASE( manage_wps, wps_tester ) try {

  auto acceptprop_param = mvo()
    ("reviewer", "reviewer1")
    ("proposal_id", 1);

  proposal_t prop;
  get_proposal(prop, N(proposer1));
  BOOST_CHECK(prop.status == PROPOSAL_STATUS_T::PENDING);

  base_tester::push_action(N(eosio.wps), N(acceptprop), N(reviewer1), acceptprop_param);
  produce_block();
  get_proposal(prop, N(proposer1));
  BOOST_CHECK(prop.status == PROPOSAL_STATUS_T::ON_VOTE);

  base_tester::push_action(N(eosio.wps), N(checkvote), N(reviewer1), acceptprop_param);
  produce_block();
  get_proposal(prop, N(proposer1));
  BOOST_CHECK(prop.status == PROPOSAL_STATUS_T::CHECK_VOTE);

  auto commitvote_param = mvo()
    ("watchman", "watchman1")
    ("proposal_id", 1);
  base_tester::push_action(N(eosio.wps), N(commitvote), N(watchman1), commitvote_param);
  produce_block();
  get_proposal(prop, N(proposer1));
  BOOST_CHECK(prop.status == PROPOSAL_STATUS_T::CHECKED_VOTE);

  base_tester::push_action(N(eosio.wps), N(approve), N(reviewer1), acceptprop_param);
  produce_block();
  get_proposal(prop, N(proposer1));
  BOOST_CHECK(prop.status == PROPOSAL_STATUS_T::APPROVED);

} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()
