#pragma once
#include <eosiolib/types.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/multi_index.hpp>
#include <eosiolib/singleton.hpp>
#include <eosiolib/contract.hpp>
#include <eosiolib/asset.hpp>

/* to generate abi, comment out*/
/*
#include "proposal.hpp"
#include "proposer.hpp"
#include "reviewer.hpp"
#include "committee.hpp"
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

    struct voter_info {
        account_name owner;
        set<uint64_t> proposals;
        uint64_t primary_key() const { return owner; }
        EOSLIB_SERIALIZE( voter_info, (owner)(proposals) )
    };

    struct wps_info {
        uint32_t lower_bound_total_voting = 5;     // 5%
        uint64_t proposal_current_index = 0;
        uint32_t max_duration = 30;                // voting duration days
        EOSLIB_SERIALIZE( wps_info, (lower_bound_total_voting)(proposal_current_index)(max_duration) )
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
                account_name committee,           // committee
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
                account_name committee,           // committee
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
            void regreviewer(account_name committee, account_name reviewer, const string& first_name, const string& last_name);

            //@abi action
            void editreviewer(account_name committee, account_name reviewer, const string& first_name, const string& last_name);

            //@abi action
            void rmvreviewer(account_name committee, const account_name reviewer);

            //@abi action
            void acceptprop(account_name reviewer, uint64_t proposal_id);

            //@abi action
            void rejectprop(account_name reviewer, uint64_t proposal_id, const string& reason);

            //@abi action
            void checkvotes(account_name reviewer, uint64_t proposal_id);

            //@abi action
            void approve(account_name reviewer, uint64_t proposal_id);

            //@abi action
            void rvmreject(account_name reviewer, uint64_t proposal_id);

            // vote
            //@abi action
            void vote(account_name voter, uint64_t proposal_id);

            //@abi action
            void unvote(account_name voter, uint64_t proposal_id);

            // watchman
            //@abi action
            void commitvote(account_name watchman, uint64_t proposal_id);

            //@abi action
            void rollbackvote(account_name watchman, uint64_t proposal_id);

            // committee
            // @abi action
            void setwpsinfo(uint32_t lower_bound_total_voting, uint32_t max_duration);

            // @abi action
            void regcommittee(account_name owner, const string& category, bool is_oversight);

            // @abi action
            void editcommittee(account_name owner, const string& category, bool is_oversight);

            // @abi action
            void rmvcommittee(account_name owner);

            //@abi action
            void rejectfunding(account_name committee, uint64_t proposal_id, const string& reason);



        private:
            wps_info_singleton m_wps_info_global;
            wps_info m_wps_info;
    };
} // eosiowps
