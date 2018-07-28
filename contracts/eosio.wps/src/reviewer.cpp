// #include <eosio.wps/eosio.wps.hpp>
#include <eosio.wps/reviewer.hpp>
#include <eosio.wps/proposal.hpp>
#include <eosio.wps/committee.hpp>

// extern struct permission_level;
// extern void require_auth(const permission_level& level);

namespace eosiowps {
	// @abi action
	void wps_contract::regreviewer(account_name committee, account_name reviewer, const string& first_name, const string& last_name) {
		//Require permission of committee account
		require_auth(committee);

		//verify that the account exists
		eosio_assert(is_account(reviewer), "The account does not exist");

		//verify that the inputs are not too short
		eosio_assert(first_name.size() > 0, "first name should be more than 0 characters long");
		eosio_assert(last_name.size() > 0, "last name should be more than 0 characters long");

		//verify that the inputs are not too long
		eosio_assert(first_name.size() < 128, "first name should be shorter than 128 characters.");
		eosio_assert(last_name.size() < 128, "last name should be shorter than 128 characters.");

        //creates the committee table if it doesn't exist already
        committee_table committees(_self, _self);

        auto itr = committees.find(committee);
        // verify that the committee is on committee table
        eosio_assert(itr != committees.end(), "Account not found in committee table");

		//creates the reviewers table if it there isn't one already
		reviewer_table reviewers(_self, _self);

		auto itr = reviewers.find(reviewer);
		// verify that the account doesn't already exist in the table
		eosio_assert(itr == reviewers.end(), "This account has already been registered as a reviewer");

		//add to the table
		reviewers.emplace(reviewer, [&](auto& reviewer){
			reviewer.owner = reviewer;
			reviewer.first_name = first_name;
			reviewer.last_name = last_name;
			reviewer.committee = committee;
		});
	}

	//@abi action
	void wps_contract::editreviewer(account_name committee, account_name reviewer, const string& first_name, const string& last_name) {
		//Require permission of committee account
		require_auth(committee);

		//verify that the account exists
		eosio_assert(is_account(reviewer), "The account does not exist");

		//verify that the inputs are not too short
		eosio_assert(first_name.size() > 0, "first name should be more than 0 characters long");
		eosio_assert(last_name.size() > 0, "last name should be more than 0 characters long");

		//verify that the inputs are not too long
		eosio_assert(first_name.size() < 128, "first name should be shorter than 128 characters.");
		eosio_assert(last_name.size() < 128, "last name should be shorter than 128 characters.");

        //creates the committee table if it doesn't exist already
        committee_table committees(_self, _self);

        auto itr = committees.find(committee);
        // verify that the committee is on committee table
        eosio_assert(itr != committees.end(), "Account not found in committee table");

		//creates the reviewers table if it there isn't one already
		reviewer_table reviewers(_self, _self);

		auto iter = reviewers.find(reviewer);
		// verify that the account already exists in the table
		eosio_assert(iter != reviewers.end(), "Account not found in reviewers table");

		// verify that the reviewer is part of the committee
        eosio_assert((*iter).committee==committee, "The given reviewer is not part of this committee");

		//add to the table
		reviewers.modify(iter, 0, [&](auto& reviewer){
			reviewer.owner = reviewer;
			reviewer.first_name = first_name;
			reviewer.last_name = last_name;
			reviewer.committee = committee;
		});
	}

	//@abi action
	void wps_contract::rmvreviewer(const account_name committee, const account_name reviewer){
		// needs authority of the committee account
		require_auth(committee);

		//verify that the account exists
		eosio_assert(is_account(reviewer), "The account does not exist");

        //creates the committee table if it doesn't exist already
        committee_table committees(_self, _self);

        auto itr = committees.find(committee);
        // verify that the committee is on committee table
        eosio_assert(itr != committees.end(), "Account not found in committee table");

		reviewer_table reviewers(_self, _self);

		auto iter = reviewers.find(reviewer);
        // verify that the account already exists in the reviewers table
		eosio_assert(iter != reviewers.end(), "Account not found in reviewers table");

        // verify that the reviewer is part of the committee
        eosio_assert((*iter).committee==committee, "The given reviewer is not part of this committee");

		reviewers.erase( iter );
	}

	//@abi action
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
		eosio_assert((*itr_proposal).committee==(*itr).committee, "Reviewer is not part of this proposal's responsible committee");

		idx_index.modify(itr_proposal, (*itr_proposal).owner, [&](auto& proposal){
			proposal.status = proposal_status::ON_VOTE;
		});
	}

	//@abi action
	void wps_contract::rejectproposal(account_name reviewer, uint64_t proposal_id, const string& reason) {
		require_auth(reviewer);

		eosio_assert(reason.size() > 0, "must provide a brief reason");
		eosio_assert(reason.size() < 256, "reason is too long");

		reviewer_table reviewers(_self, _self);
		auto itr = reviewers.find(reviewer);
		eosio_assert(itr != reviewers.end(), "Account not found in reviewers table");

		proposal_table proposals(_self, _self);
		auto idx_index = proposals.get_index<N(idx)>();
		auto itr_proposal = idx_index.find(proposal_id);
		eosio_assert(itr_proposal != idx_index.end(), "Proposal not found in proposal table");
		eosio_assert((*itr_proposal).committee==(*itr).committee, "Reviewer is not part of this proposal's responsible committee");
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

	//@abi action
	void wps_contract::approve(account_name reviewer, uint64_t proposal_id){
		require_auth(reviewer);

		reviewer_table reviewers(_self, _self);
		auto itr = reviewers.find(reviewer);
		eosio_assert(itr != reviewers.end(), "Account not found in reviewers table");

		proposal_table proposals(_self, _self);
		auto idx_index = proposals.get_index<N(idx)>();
		auto itr_proposal = idx_index.find(proposal_id);
		eosio_assert(itr_proposal != idx_index.end(), "Proposal not found in proposal table");
		eosio_assert((*itr_proposal).committee==(*itr).committee, "Reviewer is not part of this proposal's responsible committee");
		eosio_assert((*itr_proposal).status == proposal_status::FUNDED, "Proposal::status is not proposal_status::FUNDED");

		//inline action transfer
		eosio::action(
				std::vector<eosio::permission_level>(1, {_self, N(active)}),
				N(eosio.token), N(transfer), _self, (*itr_proposal).owner, (*itr_proposal).funding_goal, "Your worker proposal has been approved."
				).send();

		idx_index.erase(itr_proposal);
	}

} // eosiowps
