#include <eosio.wps/reviewer.hpp>
#include <eosio.wps/proposal.hpp>
#include <eosio.wps/committee.hpp>

namespace eosiowps {
    // @abi action
    void wps_contract::regcommittee(const account_name name, const string& category){
        //registration of committee requires contract account permissions
        require_auth(_self);

        //verify that the committee account exists
        eosio_assert(is_account(name), "committee account doesn't exist");

        //verify that the size of the category string is not too long/short
        eosio_assert(category.size() > 0, "category should be more than 0 characters long");
        eosio_assert(category.size() < 64, "category should be less than 64 characters long");

        //creates the committee table if it doesn't exist already
        committee_table committees(_self, _self);

        auto itr = committees.find(name);
        // verify that the account doesn't already exist in the table
        eosio_assert(itr == committees.end(), "This account has already been registered as a committee");

        //add to the table
        committees.emplace(name, [&](auto& committee){
            committee.name = name;
            committee.category = category;
        });
    }

    void wps_contract::editcommittee(const account_name name, const string& category){
        //editing committee info requires contract account permissions
        require_auth(_self);

        //verify that the committee account exists
        eosio_assert(is_account(name), "committee account doesn't exist");

        //verify that the size of the category string is not too long/short
        eosio_assert(category.size() > 0, "category should be more than 0 characters long");
        eosio_assert(category.size() < 64, "category should be less than 64 characters long");

        //creates the committee table if it doesn't exist already
        committee_table committees(_self, _self);

        auto itr = committees.find(name);
        // verify that the account doesn't already exist in the table
        eosio_assert(itr != committees.end(), "Account not found in committee table");

        //add to the table
        committees.modify(name, [&](auto& committee){
            committee.name = name;
            committee.category = category;
        });
    }

    void wps_contract::rmvcommittee(const account_name name){
        require_auth(_self);

        eosio_assert(is_account(name), "committee account doesn't exist");

        committee_table committees(_self, _self);

        auto itr = committees.find(name);

        eosio_assert(itr != committees.end(), "Account not found in committee table");

        committees.erase( itr );
    }
} //eosiowps