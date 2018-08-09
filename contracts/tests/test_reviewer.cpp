#include "test.hpp"

class reviewer_tester : public tester {
public:
  reviewer_tester() {
    create_accounts( {N(eosio.token), N(eosio.wps), N(eosio.saving), N(committee1), N(committee2), N(reviewer1), N(reviewer2)} );

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

    auto trace = base_tester::push_action( N(eosio.wps), N(setwpsenv), N(eosio.wps), wpsenv);
    produce_block();

    base_tester::push_action( N(eosio.wps), N(regcommittee), N(eosio.wps), committeeman1);
    base_tester::push_action( N(eosio.wps), N(regcommittee), N(eosio.wps), committeeman2);

    produce_block();
  }

  transaction_trace_ptr create_account_with_resources( account_name a, account_name creator, asset ramfunds, bool multisig,
                                                        asset net = core_from_string("10.0000"), asset cpu = core_from_string("10.0000") ) {
      signed_transaction trx;
      set_transaction_headers(trx);

      authority owner_auth;
      if (multisig) {
         // multisig between account's owner key and creators active permission
         owner_auth = authority(2, {key_weight{get_public_key( a, "owner" ), 1}}, {permission_level_weight{{creator, config::active_name}, 1}});
      } else {
         owner_auth =  authority( get_public_key( a, "owner" ) );
      }

      trx.actions.emplace_back( vector<permission_level>{{creator,config::active_name}},
                                newaccount{
                                   .creator  = creator,
                                   .name     = a,
                                   .owner    = owner_auth,
                                   .active   = authority( get_public_key( a, "active" ) )
                                });

      trx.actions.emplace_back( get_action( N(eosio), N(buyram), vector<permission_level>{{creator,config::active_name}},
                                            mvo()
                                            ("payer", creator)
                                            ("receiver", a)
                                            ("quant", ramfunds) )
                              );

      trx.actions.emplace_back( get_action( N(eosio), N(delegatebw), vector<permission_level>{{creator,config::active_name}},
                                            mvo()
                                            ("from", creator)
                                            ("receiver", a)
                                            ("stake_net_quantity", net )
                                            ("stake_cpu_quantity", cpu )
                                            ("transfer", 0 )
                                          )
                                );

      set_transaction_headers(trx);
      trx.sign( get_private_key( creator, "active" ), control->get_chain_id()  );
      return push_transaction( trx );
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

  bool get_reviewer(reviewer_t& reviewer, account_name name) {
    return get_table_entry(reviewer, N(eosio.wps), N(eosio.wps), N(reviewers), name);
  }

  abi_serializer abi_ser;
};

BOOST_AUTO_TEST_SUITE(reviewer_tests)

BOOST_FIXTURE_TEST_CASE( manage_reviewer, reviewer_tester ) try {
  auto reviewer1_1 = mvo()
    ("reviewer", "reviewer1")
    ("committee", "committee1")
    ("first_name","Thomas")
    ("last_name", "Do");

  auto reviewer2_1 = mvo()
    ("reviewer", "reviewer2")
    ("committee", "committee2")
    ("first_name","Thomas")
    ("last_name", "Cox");

  auto reviewer1_2 = mvo()
    ("reviewer", "reviewer1")
    ("committee", "committee1")
    ("first_name","Thomas2")
    ("last_name", "Do2");

  auto reviewer2_2 = mvo()
    ("reviewer", "reviewer2")
    ("committee", "committee2")
    ("first_name","Thomas2")
    ("last_name", "Cox2");

  auto r1_1 = mvo()
    ("account", "reviewer1")
    ("committee", "committee1")
    ("first_name","Thomas")
    ("last_name", "Do");

  auto r2_1 = mvo()
    ("account", "reviewer2")
    ("committee", "committee2")
    ("first_name","Thomas")
    ("last_name", "Cox");

  auto r1_2 = mvo()
    ("account", "reviewer1")
    ("committee", "committee1")
    ("first_name","Thomas2")
    ("last_name", "Do2");

  auto r2_2 = mvo()
    ("account", "reviewer2")
    ("committee", "committee2")
    ("first_name","Thomas2")
    ("last_name", "Cox2");

  base_tester::push_action( N(eosio.wps), N(regreviewer), N(committee1), reviewer1_1);
  base_tester::push_action( N(eosio.wps), N(regreviewer), N(committee2), reviewer2_1);

  produce_block();

  reviewer_t _r1_1;
  reviewer_t _r2_1;
  get_reviewer(_r1_1, N(reviewer1));
  get_reviewer(_r2_1, N(reviewer2));

  REQUIRE_MATCHING_OBJECT(_r1_1, r1_1);
  REQUIRE_MATCHING_OBJECT(_r2_1, r2_1);

  base_tester::push_action( N(eosio.wps), N(editreviewer), N(committee1), reviewer1_2);
  base_tester::push_action( N(eosio.wps), N(editreviewer), N(committee2), reviewer2_2);

  produce_block();

  reviewer_t _r1_2;
  reviewer_t _r2_2;
  get_reviewer(_r1_2, N(reviewer1));
  get_reviewer(_r2_2, N(reviewer2));

  REQUIRE_MATCHING_OBJECT(_r1_2, r1_2);
  REQUIRE_MATCHING_OBJECT(_r2_2, r2_2);

  base_tester::push_action( N(eosio.wps), N(rmvreviewer), N(committee2), mvo()("committee","committee2")("reviewer", "reviewer2"));
  produce_block();

} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()
