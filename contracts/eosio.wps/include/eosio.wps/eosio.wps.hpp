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

	const uint32_t seconds_per_day = 60 * 60 * 24;

    //@abi table
    struct voting_info {
        uint64_t proposal_id;
        vector<account_name> agrees;
        vector<account_name> disagrees;
        uint64_t primary_key() const { return proposal_id; }
        EOSLIB_SERIALIZE( voting_info, (proposal_id)(agrees)(disagrees) )
    };

    //@abi table
    struct wps_env {
        uint64_t proposal_current_index = 0;
        uint32_t total_voting_percent = 5;                      // 5%
        uint32_t duration_of_voting = 30 * seconds_per_day;     // voting duration (seconds)
        uint32_t duration_of_funding = 180 * seconds_per_day;   // funding duration (seconds)
        uint32_t total_iteration_of_funding = 6;                //
        EOSLIB_SERIALIZE( wps_env, (proposal_current_index)(total_voting_percent)(duration_of_voting)(duration_of_funding)(total_iteration_of_funding) )
    };

    typedef eosio::multi_index< N(voting), voting_info > voting_table;
    typedef eosio::singleton< N(wpsglobal), wps_env > wps_env_singleton;

    class wps_contract : public eosio::contract {
        public:
            explicit wps_contract(action_name self);

            // proposer
            // @abi action
            void regproposer(account_name account, const string& first_name, const string& last_name,
                            const string& img_url, const string& bio, const string& country, const string& telegram,
                            const string& website, const string& linkedin);

            //@abi action
            void editproposer(account_name account, const string& first_name, const string& last_name,
                            const string& img_url, const string& bio, const string& country, const string& telegram,
                            const string& website, const string& linkedin);

            //@abi action
            void rvmproposer(account_name account);

            //@abi action
            void claimfunds(account_name account, uint64_t proposal_id);

            // proposal
            // @abi action
            void regproposal(
                account_name proposer,
                account_name committee,           // committee
                uint16_t subcategory,             // subcategory
                const string& title,              // title
                const string& subtitle,           // subtitle
                const string& project_img_url,    // project image or video url
                const string& project_overview,   // overview
                const string& financial_loadmap,  // financial
                const vector<string>& members,     // linkedin
                const asset& funding_goal
            );

            //@abi action
            void editproposal(
                account_name proposer,
                account_name committee,           // committee
                uint16_t subcategory,             // subcategory
                const string& title,              // title
                const string& subtitle,           // subtitle
                const string& project_img_url,    // project image or video url
                const string& project_overview,   // overview
                const string& financial_loadmap,  // financial
                const vector<string>& members,    // linkedin
                const asset& funding_goal
            );

            //@abi action
            void rmvproposal(account_name proposer);

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
            void checkvote(account_name reviewer, uint64_t proposal_id);

            //@abi action
            void approve(account_name reviewer, uint64_t proposal_id);

            //@abi action
            void rmvreject(account_name reviewer, uint64_t proposal_id);

            // @abi action
            void rmvcompleted(account_name reviewer, uint64_t proposal_id);


            // vote
            //@abi action
            void vote(account_name voter, uint64_t proposal_id, bool is_agree);

            //@abi action
            void unvote(account_name voter, uint64_t proposal_id);


            // committee
            // @abi action
            void setwpsenv(uint32_t total_voting_percent, uint32_t duration_of_voting, uint32_t duration_of_funding, uint32_t iteration_of_funding);

            // @abi action
            void regcommittee(account_name committeeman, const string& category, bool is_oversight);

            // @abi action
            void editcommittee(account_name committeeman, const string& category, bool is_oversight);

            // @abi action
            void rmvcommittee(account_name committeeman);

            //@abi action
            void rejectfunding(account_name committeeman, uint64_t proposal_id, const string& reason);


            // watchman
            //@abi action
            void commitvote(account_name watchman, uint64_t proposal_id);

            //@abi action
            void rollbackvote(account_name watchman, uint64_t proposal_id);

            //@abi action
            void checkexpire(account_name watchman, uint64_t proposal_id);

        private:
            wps_env_singleton m_wps_env_global;
    };
} // eosiowps
