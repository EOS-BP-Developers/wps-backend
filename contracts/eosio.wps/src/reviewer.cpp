// #include <eosio.wps/eosio.wps.hpp>
#include <eosio.wps/reviewer.hpp>
#include <eosio.wps/proposal.hpp>

// extern struct permission_level;
// extern void require_auth(const permission_level& level);

namespace eosiowps {
	// @abi action
	void wps_contract::regreviewer(account_name owner, const string& first_name, const string& last_name, const string& committee) {
		//Require permission of contract account
		require_auth(_self);

		//verify that the account exists
		eosio_assert(is_account(owner), "The account does not exist");

		//verify that the inputs are not too short
		eosio_assert(first_name.size() > 0, "first name should be more than 0 characters long");
		eosio_assert(last_name.size() > 0, "last name should be more than 0 characters long");
		eosio_assert(committee.size() > 0, "committee name should be more than 0 characters long");

		//verify that the inputs are not too long
		eosio_assert(first_name.size() < 128, "first name should be shorter than 128 characters.");
		eosio_assert(last_name.size() < 128, "last name should be shorter than 128 characters.");
		eosio_assert(committee.size() < 64, "committee name should be shorter than 64 charactrs.");

		//creates the reviewers table if it there isn't one already
		reviewer_table reviewers(_self, _self);

		auto itr = reviewers.find(owner);
		// verify that the account doesn't already exist in the table
		eosio_assert(itr == reviewers.end(), "This account has already been registered as a reviewer");

		//add to the table
		reviewers.emplace(owner, [&](auto& reviewer){
			reviewer.owner = owner;
			reviewer.first_name = first_name;
			reviewer.last_name = last_name;
			reviewer.committee = committee;
		});
	}

	//@abi action
	void wps_contract::editreviewer(account_name owner, const string& first_name, const string& last_name, const string& committee) {
		//Require permission of contract account
		require_auth(_self);

		//verify that the account exists
		eosio_assert(is_account(owner), "The account does not exist");

		//verify that the inputs are not too short
		eosio_assert(first_name.size() > 0, "first name should be more than 0 characters long");
		eosio_assert(last_name.size() > 0, "last name should be more than 0 characters long");
		eosio_assert(committee.size() > 0, "committee name should be more than 0 characters long");

		//verify that the inputs are not too long
		eosio_assert(first_name.size() < 128, "first name should be shorter than 128 characters.");
		eosio_assert(last_name.size() < 128, "last name should be shorter than 128 characters.");
		eosio_assert(committee.size() < 64, "committee name should be shorter than 64 charactrs.");

		//creates the reviewers table if it there isn't one already
		reviewer_table reviewers(_self, _self);

		auto itr = reviewers.find(owner);
		// verify that the account already exists in the table
		eosio_assert(itr != reviewers.end(), "Account not found in reviewers table");

		//add to the table
		reviewers.modify(itr, owner, [&](auto& reviewer){
			reviewer.owner = owner;
			reviewer.first_name = first_name;
			reviewer.last_name = last_name;
			reviewer.committee = committee;
		});
	}

	//@abi action
	void wps_contract::rmvreviewer(const account_name owner){
		// needs authority of the contract account
		require_auth(_self);

		//verify that the account exists
		eosio_assert(is_account(owner), "The account does not exist");

		reviewer_table reviewers(_self, _self);

		// verify that the account already exists in the reviewers table
		auto itr = reviewers.find(owner);
		eosio_assert(itr != reviewers.end(), "Account not found in reviewers table");

		reviewers.erase( itr );
	}

	void wps_contract::acceptproposal(const account_name reviewer, const uint64_t& proposal_id){
		require_auth(_self);

		eosio_assert(is_account(reviewer), "The account does not exist");

		auto itr = reviewers.find(reviewer);
		eosio_assert(itr != reviewers.end(), "Account not found in reviewers table");

		proposal_table proposals(_self, _self);

		auto id_index = proposals.get_index<N(idx)>();
		auto iter = id_index.lower_bound(proposal_id);
		for(; iter != id_index.end() && iter->id == proposal_id; ++iter){
			id_index.modify(iter, 0, [&](auto &proposal){
				proposal.status = 1;
			});
		}

		eosio_assert(iter != id_index.end(), "proposal with given ID doesn't exist");
	}

	void wps_contract::rejectproposal(const account_name reviewer, const uint64_t& proposal_id){
		require_auth(_self);

		eosio_assert(is_account(reviewer), "The account does not exist");

		auto itr = reviewers.find(reviewer);
		eosio_assert(itr != reviewers.end(), "Account not found in reviewers table");

		proposal_table proposals(_self, _self);

		auto id_index = proposals.get_index<N(idx)>();
		auto iter = id_index.lower_bound(proposal_id);
		for(; iter != id_index.end() && iter->id == proposal_id; ++iter) {
			id_index.modify(iter, 0, [&](auto &proposal) {
				proposal.status = 2;
			});
		}

		eosio_assert(iter != id_index.end(), "proposal with given ID doesn't exist");
	}

} // eosiowps
