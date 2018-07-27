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

	void wps_contract::acceptproposal(account_name reviewer, uint64_t proposal_id) {
		require_auth(reviewer);

		reviewer_table reviewers(_self, _self);
		auto itr = reviewers.find(reviewer);
		eosio_assert(itr != reviewers.end(), "Account not found in reviewers table");

		proposal_table proposals(_self, _self);
		auto idx_index = proposals.get_index<N(idx)>();
		auto itr_proposal = idx_index.find(proposal_id);
		eosio_assert(itr_proposal != idx_index.end(), "Proposal not found in proposal table");
		eosio_assert((*itr_proposal).status == proposal_status::PENDING, "Proposal::status is not proposal_status::PENDING");

		idx_index.modify(itr_proposal, (*itr_proposal).owner, [&](auto& proposal){
			proposal.status = proposal_status::ON_VOTE;
		});
	}

	void wps_contract::rejectproposal(account_name reviewer, uint64_t proposal_id) {
		require_auth(reviewer);

		reviewer_table reviewers(_self, _self);
		auto itr = reviewers.find(reviewer);
		eosio_assert(itr != reviewers.end(), "Account not found in reviewers table");

		proposal_table proposals(_self, _self);
		auto idx_index = proposals.get_index<N(idx)>();
		auto itr_proposal = idx_index.find(proposal_id);
		eosio_assert(itr_proposal != idx_index.end(), "Proposal not found in proposal table");
		// eosio_assert((*itr_proposal).status == proposal_status::PENDING, "Proposal::status is not proposal_status::PENDING");

		idx_index.modify(itr_proposal, (*itr_proposal).owner, [&](auto& proposal){
			proposal.status = proposal_status::REJECT;
		});

		rejected_proposal_table rejected_proposals(_self, _self);

		//add to the table
		rejected_proposals.emplace((*itr_proposal).owner, [&](auto& proposal){
			proposal = std::move(*itr_proposal);
		});

		idx_index.erase(itr_proposal);
	}

} // eosiowps
