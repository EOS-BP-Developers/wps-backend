#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>

#include <eosio/testing/tester.hpp>
#include <eosio/chain/abi_serializer.hpp>
#include <eosio/chain/wast_to_wasm.hpp>

#include <fc/variant_object.hpp>
#include <fc/log/logger.hpp>
#include <eosio/chain/exceptions.hpp>

#include <Runtime/Runtime.h>

#include "contracts.hpp"

using namespace eosio::testing;
using namespace eosio;
using namespace eosio::chain;
using namespace eosio::testing;
using namespace fc;

using mvo = fc::mutable_variant_object;

class eosio_wps_tester : public tester {
public:
   eosio_wps_tester() {
      create_accounts( {N(eosio.token), N(eosio.wps), N(eosio.saving), N(alice), N(bob), N(carol) } );
      produce_block();

      auto trace1 = base_tester::push_action(config::system_account_name, N(setpriv),
                                            config::system_account_name, mutable_variant_object()
                                            ("account", "eosio.token")
                                            ("is_priv", 1)
      );

      auto trace2 = base_tester::push_action(config::system_account_name, N(setpriv),
                                            config::system_account_name, mutable_variant_object()
                                            ("account", "eosio.wps")
                                            ("is_priv", 1)
      );

      auto trace3 = base_tester::push_action(config::system_account_name, N(setpriv),
                                            config::system_account_name, mutable_variant_object()
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

      // BOOST_TEST_MESSAGE( "Current time:" << fc::json::to_pretty_string(trace1) );
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

   transaction_trace_ptr push_action( const account_name& signer, const action_name& name, const variant_object& data, bool auth = true ) {
      vector<account_name> accounts;
      if( auth )
         accounts.push_back( signer );
      auto trace = base_tester::push_action( N(eosio.msig), name, accounts, data );
      produce_block();
      BOOST_REQUIRE_EQUAL( true, chain_has_transaction(trace->id) );
      return trace;
   }

   transaction reqauth( account_name from, const vector<permission_level>& auths );

   abi_serializer abi_ser;
};

transaction eosio_wps_tester::reqauth( account_name from, const vector<permission_level>& auths ) {
   fc::variants v;
   for ( auto& level : auths ) {
      v.push_back(mvo()
                  ("actor", level.actor)
                  ("permission", level.permission)
      );
   }
   variant pretty_trx = mvo()
      ("expiration", "2020-01-01T00:30")
      ("ref_block_num", 2)
      ("ref_block_prefix", 3)
      ("max_net_usage_words", 0)
      ("max_cpu_usage_ms", 0)
      ("delay_sec", 0)
      ("actions", fc::variants({
            mvo()
               ("account", name(config::system_account_name))
               ("name", "reqauth")
               ("authorization", v)
               ("data", mvo() ("from", from) )
               })
      );
   transaction trx;
   abi_serializer::from_variant(pretty_trx, trx, get_resolver(), fc::seconds(1));
   return trx;
}

BOOST_AUTO_TEST_SUITE(eosio_wps_tests)

BOOST_FIXTURE_TEST_CASE( register_proposer, eosio_wps_tester ) try {
} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( edit_proposer, eosio_wps_tester ) try {

} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( remve_proposer, eosio_wps_tester ) try {

} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( register_proposal, eosio_wps_tester ) try {

} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( edit_proposal, eosio_wps_tester ) try {

} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( remove_proposal, eosio_wps_tester ) try {

} FC_LOG_AND_RETHROW()


BOOST_AUTO_TEST_SUITE_END()

void translate_fc_exception(const fc::exception &e) {
   std::cerr << "\033[33m" <<  e.to_detail_string() << "\033[0m" << std::endl;
   BOOST_TEST_FAIL("Caught Unexpected Exception");
}

boost::unit_test::test_suite* init_unit_test_suite(int argc, char* argv[]) {
   // Turn off blockchain logging if no --verbose parameter is not added
   // To have verbose enabled, call "tests/chain_test -- --verbose"
   bool is_verbose = false;
   std::string verbose_arg = "--verbose";
   for (int i = 0; i < argc; i++) {
      if (verbose_arg == argv[i]) {
         is_verbose = true;
         break;
      }
   }
   if(!is_verbose) fc::logger::get(DEFAULT_LOGGER).set_log_level(fc::log_level::off);

   // Register fc::exception translator
   boost::unit_test::unit_test_monitor.template register_exception_translator<fc::exception>(&translate_fc_exception);

   std::srand(time(NULL));
   std::cout << "Random number generator seeded to " << time(NULL) << std::endl;
   return nullptr;
}