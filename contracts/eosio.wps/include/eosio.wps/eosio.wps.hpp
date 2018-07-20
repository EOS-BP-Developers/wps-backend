#pragma once
#include <eosiolib/types.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/multi_index.hpp>
#include <eosiolib/contract.hpp>
#include <eosiolib/asset.hpp>

/*
#include "proposal.hpp"
#include "proposer.hpp"
#include "reviewer.hpp"
*/

#include <string>
#include <vector>

namespace eosiowps {
    using std::string;
    using std::vector;

    using eosio::asset;
    using eosio::indexed_by;
    using eosio::const_mem_fun;

    //@abi table
    struct voter_info {
        account_name owner;
        vector<uint32_t> proposals;
        uint64_t primary_key() const { return owner; }
        EOSLIB_SERIALIZE( voter_info, (owner)(proposals) )
    };

    struct wps_info {
        uint32_t lower_bound_total_voting;
    };

    typedef eosio::multi_index< N(voter), voter_info > voter_table;

    class wps_contract : public eosio::contract {
        public:
            explicit wps_contract(action_name self) : contract(self){
            }

            // proposer
            // @abi action
            void regproposer(account_name owner, const string& first_name, const string& last_name,
                            const string& img_url, const string& bio, const string& country, const string& telegram,
                            const string& website, const string& linkedin);

            //@abi action
            void editproposer(account_name owner, const string& first_name, const string& last_name,
                            const string& img_url, const string& bio, const string& country, const string& telegram,
                            const string& website, const string& linkedin);

            //@abi action
            void rmvproposer(const account_name owner);

            // proposal
            // @abi action
            void regproposal(account_name owner,
                uint16_t category,                // category
                uint16_t subcategory,             // subcategory
                const string& title,              // title
                const string& subtitle,           // subtitle
                const string& project_img_url,    // project image or video url
                const string& project_overview,   // overview
                const string& financial_loadmap,  // financial
                const vector<string>& members,     // linkedin
                const asset& funding_goal,
                uint16_t duration
            );

            //@abi action
            void editproposal(account_name owner,
                uint16_t category,                // category
                uint16_t subcategory,             // subcategory
                const string& title,              // title
                const string& subtitle,           // subtitle
                const string& project_img_url,    // project image or video url
                const string& project_overview,   // overview
                const string& financial_loadmap,  // financial
                const vector<string>& members,    // linkedin
                const asset& funding_goal,
                uint16_t duration
            );

            //@abi action
            void rmvproposal(const account_name owner);

            // reviewer
            //@abi action
            void regreviewer(account_name account, const string& first_name, const string& last_name, const string& committee);

            //@abi action
            void editreviewer(account_name account, const string& first_name, const string& last_name, const string& committee);

            //@abi action
            void rmvreviewer(const account_name account);
    };
} // eosiowps
