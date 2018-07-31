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
        const static uint8_t CHECK_VOTE = 4;     // check count of votes
        const static uint8_t CHECKED_VOTE = 5;   // checked count of votes by platform
        const static uint8_t APPROVED = 6;       // approve
        const static uint8_t ON_FUND = 7;
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
        string subtitle;              // subtitle
        string project_img_url;       // project image or video url
        string project_overview;      // overview
        string financial_roadmap;     // financial
        vector<string> members;       // linkedin
        asset funding_goal;           // amount of EOS
        uint16_t duration;            // voting duration
        uint64_t total_votes;         // total votes
        uint8_t status;               // status
        uint8_t iteration_of_funding; // number of iteration
        uint64_t primary_key() const { return proposer; }
        uint64_t by_id() const { return static_cast<uint64_t>(id); }
        EOSLIB_SERIALIZE( proposal, (proposer)(id)(committee)(category)(subcategory)(title)(subtitle)(project_img_url)(project_overview)(financial_roadmap)(members)(total_votes)(status)(iteration_of_funding) )
    };

    typedef eosio::multi_index< N(proposals), proposal,
        indexed_by< N(idx), const_mem_fun<proposal, uint64_t, &proposal::by_id>  >
    > proposal_table;

    typedef eosio::multi_index< N(rejectedprop), proposal,
        indexed_by< N(idx), const_mem_fun<proposal, uint64_t, &proposal::by_id>  >
    > rejected_proposal_table;

    typedef eosio::multi_index< N(fundedprop), proposal,
        indexed_by< N(idx), const_mem_fun<proposal, uint64_t, &proposal::by_id>  >
    > funded_proposal_table;

} // eosiowps