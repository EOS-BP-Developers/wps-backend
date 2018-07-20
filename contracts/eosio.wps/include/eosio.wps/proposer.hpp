#pragma once

#include <eosiolib/eosio.hpp>

#include <string>
#include <vector>

namespace eosiowps {
    using std::string;
    using std::vector;

    //@abi table
    struct proposer {
        account_name owner;
        string first_name;
        string last_name;
        string img_url;
        string bio;
        string country;
        string telegram;
        string website;
        string linkedin;
        uint64_t primary_key() const { return owner; }
        EOSLIB_SERIALIZE( proposer, (owner)(first_name)(last_name)(img_url)(bio)(country)(telegram)(website)(linkedin) )
    };
    typedef eosio::multi_index<N(proposer), proposer> proposer_table;

} // eosiowps