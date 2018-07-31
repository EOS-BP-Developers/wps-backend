#include <eosio.wps/reviewer.hpp>
#include <eosio.wps/proposal.hpp>
#include <eosio.wps/committee.hpp>

namespace eosiowps {

    // @abi action
    void wps_contract::setwpsinfo(/*account_name watchman,*/ uint32_t lower_bound_total_voting, uint32_t max_duration) {
        //registration of committee requires contract account permissions
        require_auth(_self);

        //eosio_assert(is_account(watchman), "committee account doesn't exist");
        eosio_assert(lower_bound_total_voting >= 5, "lower_bound_total_voting should be more than equal 5 long");
        eosio_assert(max_duration >= 60, "max_duration should be more than equal 60 long");

        m_wps_info = m_wps_info_global.exists() ? m_wps_info_global.get() : wps_info();

        //m_wps_info.watchman = watchman;
        m_wps_info.lower_bound_total_voting = lower_bound_total_voting;
        m_wps_info.max_duration = max_duration;

        m_wps_info_global.set( m_wps_info, _self );
    }

    // @abi action
    void wps_contract::regcommittee(account_name owner, const string& category, bool is_oversight) {
        //registration of committee requires contract account permissions
        require_auth(_self);

        //verify that the committee account exists
        eosio_assert(is_account(owner), "committee account doesn't exist");

        //verify that the size of the category string is not too long/short
        eosio_assert(category.size() > 0, "category should be more than 0 characters long");
        eosio_assert(category.size() < 64, "category should be less than 64 characters long");

        //creates the committee table if it doesn't exist already
        committee_table committees(_self, _self);

        auto itr = committees.find(owner);
        // verify that the account doesn't already exist in the table
        eosio_assert(itr == committees.end(), "This account has already been registered as a committee");

        //add to the table
        committees.emplace(owner, [&](auto& committee){
            committee.owner = owner;
            committee.category = category;
            committee.is_oversight = is_oversight;
        });
    }

    void wps_contract::editcommittee(account_name owner, const string& category, bool is_oversight) {
        //editing committee info requires contract account permissions
        require_auth(_self);

        //verify that the committee account exists
        eosio_assert(is_account(owner), "committee account doesn't exist");

        //verify that the size of the category string is not too long/short
        eosio_assert(category.size() > 0, "category should be more than 0 characters long");
        eosio_assert(category.size() < 64, "category should be less than 64 characters long");

        //creates the committee table if it doesn't exist already
        committee_table committees(_self, _self);

        auto itr = committees.find(owner);
        // verify that the account doesn't already exist in the table
        eosio_assert(itr != committees.end(), "Account not found in committee table");

        //add to the table
        committees.modify(itr, owner, [&](auto& committee){
            committee.owner = owner;
            committee.category = category;
            committee.is_oversight = is_oversight;
        });
    }

    void wps_contract::rmvcommittee(account_name owner) {
        require_auth(_self);

        eosio_assert(is_account(owner), "committee account doesn't exist");

        committee_table committees(_self, _self);
        auto itr = committees.find(owner);

        eosio_assert(itr != committees.end(), "Account not found in committee table");
        committees.erase( itr );
    }
} //eosiowps