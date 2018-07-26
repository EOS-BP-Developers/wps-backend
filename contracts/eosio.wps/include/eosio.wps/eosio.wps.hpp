#pragma once
#include <eosiolib/types.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/multi_index.hpp>
#include <eosiolib/singleton.hpp>
#include <eosiolib/contract.hpp>
#include <eosiolib/asset.hpp>

/* to generate abi, comment out
#include "proposal.hpp"
#include "proposer.hpp"
#include "reviewer.hpp"
*/

#include <string>
#include <vector>

namespace eosiowps {
    using std::string;
    using std::vector;
    using std::set;

    using eosio::asset;
    using eosio::indexed_by;
    using eosio::const_mem_fun;

    //@abi table
    struct voter_info {
        account_name owner;
        set<uint64_t> proposals;
        uint64_t primary_key() const { return owner; }
        EOSLIB_SERIALIZE( voter_info, (owner)(proposals) )
    };

    struct wps_info {
        uint32_t lower_bound_total_voting = 50000; // sample
        uint64_t proposal_current_index = 0;
        uint32_t max_duration = 60;                // days
    };

    typedef eosio::multi_index< N(voter), voter_info > voter_table;
    typedef eosio::singleton< N(wpsglobal), wps_info > wps_info_singleton;

    class wps_contract : public eosio::contract {
        public:
            explicit wps_contract(action_name self);

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

            //@abi action
            void acceptproposal(account_name reviewer, uint64_t proposal_id);

            //@abi action
            void rejectproposal(account_name reviewer, uint64_t proposal_id);

            // vote
            //@abi action
            void vote(account_name voter, uint64_t proposal_id);

            //@abi action
            void unvote(account_name voter, uint64_t proposal_id);

        private:
            wps_info_singleton m_wps_info_global;
            wps_info m_wps_info;
    };
} // eosiowps
