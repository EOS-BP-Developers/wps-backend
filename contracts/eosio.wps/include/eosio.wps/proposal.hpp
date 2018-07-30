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

    struct proposal_status {
        const static uint8_t PENDING = 1;
        const static uint8_t REJECT = 2;
        const static uint8_t ON_VOTE = 3;
        const static uint8_t FUNDED = 4;
        const static uint8_t APPROVED = 5;
    };

    //@abi table
    struct proposal {
        account_name owner;        // proposer
        uint64_t id;
        account_name committee;    // committee
        string category;           // category
        uint16_t subcategory;      // subcategory
        string title;              // title
        string subtitle;           // subtitle
        string project_img_url;    // project image or video url
        string project_overview;   // overview
        string financial_roadmap;  // financial
        vector<string> members;    // linkedin
        asset funding_goal;        // amount of EOS
        uint16_t duration;         // voting duration
        uint32_t total_votes;
        uint8_t status;            // review status
        uint64_t primary_key() const { return owner; }
        uint64_t by_id() const { return static_cast<uint64_t>(id); }
        EOSLIB_SERIALIZE( proposal, (owner)(id)(committee)(category)(subcategory)(title)(subtitle)(project_img_url)(project_overview)(financial_roadmap)(members)(total_votes)(status) )
    };

    typedef eosio::multi_index< N(proposals), proposal,
        indexed_by< N(idx), const_mem_fun<proposal, uint64_t, &proposal::by_id>  >
    > proposal_table;

    typedef eosio::multi_index< N(rejectedprop), proposal,
        indexed_by< N(idx), const_mem_fun<proposal, uint64_t, &proposal::by_id>  >
    > rejected_proposal_table;

    typedef eosio::multi_index< N(approvedprop), proposal,
        indexed_by< N(idx), const_mem_fun<proposal, uint64_t, &proposal::by_id>  >
    > approved_proposal_table;

} // eosiowps