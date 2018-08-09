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

    create_accounts( {N(voter1), N(voter2), N(voter3), N(voter4), N(voter5)});

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



    create_currency(N(eosio.token), N(eosio), core_from_string("10000000000.0000"));
    produce_blocks();
    issue(N(eosio), core_from_string("100000000.0000"));
    transfer(N(eosio), N(eosio.wps), "100000.0000 EOS");

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

  void create_currency( name contract, name manager, asset maxsupply ) {
    auto act =  mutable_variant_object()
      ("issuer",       manager )
      ("maximum_supply", maxsupply );

    base_tester::push_action(contract, N(create), contract, act );
  }

  void issue( name to, const asset& amount, name manager = config::system_account_name ) {
    base_tester::push_action( N(eosio.token), N(issue), manager, mutable_variant_object()
                              ("to",      to )
                              ("quantity", amount )
                              ("memo", "")
                              );
  }

   void transfer( name from, name to, const string& amount, name manager = config::system_account_name ) {
      base_tester::push_action( N(eosio.token), N(transfer), manager, mutable_variant_object()
                                ("from",    from)
                                ("to",      to )
                                ("quantity", asset::from_string(amount) )
                                ("memo", "")
                                );
   }

   asset get_balance( const account_name& act ) {
      //return get_currency_balance( config::system_account_name, symbol(CORE_SYMBOL), act );
      //temporary code. current get_currency_balancy uses table name N(accounts) from currency.h
      //generic_currency table name is N(account).
      const auto& db  = control->db();
      const auto* tbl = db.find<table_id_object, by_code_scope_table>(boost::make_tuple(N(eosio.token), act, N(accounts)));
      share_type result = 0;

      // the balance is implied to be 0 if either the table or row does not exist
      if (tbl) {
         const auto *obj = db.find<key_value_object, by_scope_primary>(boost::make_tuple(tbl->id, symbol(CORE_SYMBOL).to_symbol_code()));
         if (obj) {
            // balance is the first field in the serialization
            fc::datastream<const char *> ds(obj->value.data(), obj->value.size());
            fc::raw::unpack(ds, result);
         }
      }
      return asset( result, symbol(CORE_SYMBOL) );
  }

  bool get_proposal(proposal_t& proposal, account_name name) {
    return get_table_entry(proposal, N(eosio.wps), N(eosio.wps), N(proposals), name);
  }

  bool get_voting_info(voting_info_t& voting_info, uint64_t proposal_id) {
    return get_table_entry(voting_info, N(eosio.wps), N(eosio.wps), N(votings), proposal_id);
  }

  fc::variant get_account( account_name account, const string& symbolname)
  {
    abi_serializer abi_ser;
    const auto& acc = control->db().get<account_object,by_name>( N(eosio.token) );
    abi_def abi;
    BOOST_REQUIRE_EQUAL(abi_serializer::to_abi(acc.abi, abi), true);
    abi_ser.set_abi(abi, fc::seconds(1));

    auto symb = eosio::chain::symbol::from_string(symbolname);
    auto symbol_code = symb.to_symbol_code().value;
    vector<char> data = get_row_by_account( N(eosio.token), account, N(accounts), symbol_code );
    return data.empty() ? fc::variant() : abi_ser.binary_to_variant( "account", data, fc::microseconds(30) );
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

  produce_blocks(10);
  base_tester::push_action(N(eosio.wps), N(vote), N(voter1), mvo()("voter", "voter1")("proposal_id", 1)("is_agree", true));
  produce_blocks(10);
  base_tester::push_action(N(eosio.wps), N(unvote), N(voter1), mvo()("voter", "voter1")("proposal_id", 1));
  produce_blocks(10);

  base_tester::push_action(N(eosio.wps), N(vote), N(voter1), mvo()("voter", "voter1")("proposal_id", 1)("is_agree", true));
  produce_blocks(10);
  base_tester::push_action(N(eosio.wps), N(vote), N(voter2), mvo()("voter", "voter2")("proposal_id", 1)("is_agree", true));
  produce_blocks(10);
  base_tester::push_action(N(eosio.wps), N(vote), N(voter3), mvo()("voter", "voter3")("proposal_id", 1)("is_agree", false));
  produce_blocks(10);
  base_tester::push_action(N(eosio.wps), N(vote), N(voter4), mvo()("voter", "voter4")("proposal_id", 1)("is_agree", true));
  produce_blocks(10);
  base_tester::push_action(N(eosio.wps), N(vote), N(voter5), mvo()("voter", "voter5")("proposal_id", 1)("is_agree", true));
  produce_blocks(10);

  voting_info_t vt;
  get_voting_info(vt, 1);
  BOOST_TEST_MESSAGE(fc::json::to_pretty_string(vt));

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
  produce_blocks(10);
  get_proposal(prop, N(proposer1));
  BOOST_CHECK(prop.status == PROPOSAL_STATUS_T::APPROVED);

  base_tester::push_action(N(eosio.wps), N(claimfunds), N(proposer1), mvo()("account", "proposer1")("proposal_id", 1));
  produce_block();
  get_proposal(prop, N(proposer1));
  BOOST_TEST_MESSAGE(fc::json::to_pretty_string(prop));

  auto acc = get_account(N(proposer1), "4,EOS");
  BOOST_TEST_MESSAGE(fc::json::to_pretty_string(acc));

  // base_tester::push_action(N(eosio.wps), N(claimfunds), N(proposer1), mvo()("account", "proposer1")("proposal_id", 1));

} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()
