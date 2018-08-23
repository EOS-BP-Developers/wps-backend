#pragma once

#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>

#include <string>
#include <vector>

namespace eosiowps {
    using eosio::asset;
    using eosio::indexed_by;
    using eosio::const_mem_fun;
    using std::string;
    using std::vector;

    struct PROPOSAL_STATUS {
        const static uint8_t PENDING = 1;
        const static uint8_t REJECTED = 2;
        const static uint8_t ON_VOTE = 3;
        const static uint8_t FINISHED_VOTING = 4;
        const static uint8_t CHECK_VOTE = 5;     // check count of votes
        const static uint8_t CHECKED_VOTE = 6;   // checked count of votes by platform
        const static uint8_t APPROVED = 7;       // approve
        const static uint8_t COMPLETED = 8;
    };

    //@abi table
    struct proposal {
        account_name proposer;        // proposer
        uint64_t id;
        account_name committee;       // committee
        string category;              // category
        uint16_t subcategory;         // subcategory
        string title;                 // title
        string summary;               // summary
        string project_img_url;       // project image or video url
        string description;           // overview
        string roadmap;               // roadmap
        uint64_t duration;            // duration
        vector<string> members;       // linkedin
        asset funding_goal;           // amount of EOS
        uint64_t total_votes;         // total votes
        uint64_t agree_votes;         // agree votes
        uint64_t disagree_votes;      // disagree votes
        uint8_t status;               // status
        uint64_t vote_start_time;     // time when voting starts (seconds)
        uint64_t fund_start_time;     // time when funding starts (seconds)
        uint8_t iteration_of_funding; // number of iteration
        uint64_t primary_key() const { return proposer; }
        uint64_t by_id() const { return static_cast<uint64_t>(id); }
        EOSLIB_SERIALIZE( proposal, (proposer)(id)(committee)(category)(subcategory)(title)(summary)(project_img_url)(description)(roadmap)(duration)(members)(funding_goal)
            (total_votes)(agree_votes)(disagree_votes)(status)(vote_start_time)(fund_start_time)(iteration_of_funding) )
    };

    typedef eosio::multi_index< N(proposals), proposal,
        indexed_by< N(idx), const_mem_fun<proposal, uint64_t, &proposal::by_id>  >
    > proposal_table;

    typedef eosio::multi_index< N(rejectedpros), proposal,
        indexed_by< N(idx), const_mem_fun<proposal, uint64_t, &proposal::by_id>  >
    > rejected_proposal_table;

    typedef eosio::multi_index< N(finishedpros), proposal,
        indexed_by< N(idx), const_mem_fun<proposal, uint64_t, &proposal::by_id>  >
    > finished_proposal_table;

} // eosiowps