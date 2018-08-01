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

		auto reviewer_itr = reviewers.find(reviewer);
		// verify that the account doesn't already exist in the table
		eosio_assert(reviewer_itr == reviewers.end(), "This account has already been registered as a reviewer");

		//add to the table
		reviewers.emplace(committee, [&](auto& _reviewer){
			_reviewer.account = reviewer;
			_reviewer.first_name = first_name;
			_reviewer.last_name = last_name;
			_reviewer.committee = committee;
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
        auto committee_itr = committees.find(committee);
        // verify that the committee is on committee table
        eosio_assert(committee_itr != committees.end(), "Account not found in committee table");

		//creates the reviewers table if it there isn't one already
		reviewer_table reviewers(_self, _self);
		auto itr = reviewers.find(reviewer);
		// verify that the account already exists in the table
		eosio_assert(itr != reviewers.end(), "Account not found in reviewers table");
		// verify that the reviewer is part of the committee
        eosio_assert((*itr).committee == committee, "The given reviewer is not part of this committee");

		//add to the table
		reviewers.modify(itr, 0, [&](auto& _reviewer){
			_reviewer.account = reviewer;
			_reviewer.first_name = first_name;
			_reviewer.last_name = last_name;
			_reviewer.committee = committee;
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
        auto committee_itr = committees.find(committee);
        // verify that the committee is on committee table
        eosio_assert(committee_itr != committees.end(), "Account not found in committee table");

		reviewer_table reviewers(_self, _self);
		auto itr = reviewers.find(reviewer);
        // verify that the account already exists in the reviewers table
		eosio_assert(itr != reviewers.end(), "Account not found in reviewers table");
        // verify that the reviewer is part of the committee
        eosio_assert((*itr).committee==committee, "The given reviewer is not part of this committee");

		reviewers.erase( itr );
	}

	//@abi action
	void wps_contract::acceptprop(account_name reviewer, uint64_t proposal_id) {
		require_auth(reviewer);

		reviewer_table reviewers(_self, _self);
		auto itr = reviewers.find(reviewer);
		eosio_assert(itr != reviewers.end(), "Account not found in reviewers table");

		proposal_table proposals(_self, _self);
		auto idx_index = proposals.get_index<N(idx)>();
		auto itr_proposal = idx_index.find(proposal_id);
		eosio_assert(itr_proposal != idx_index.end(), "Proposal not found in proposal table");
		eosio_assert((*itr_proposal).status == PROPOSAL_STATUS::PENDING, "Proposal::status is not proposal_status::PENDING");
		eosio_assert((*itr_proposal).committee==(*itr).committee, "Reviewer is not part of this proposal's responsible committee");

		idx_index.modify(itr_proposal, 0, [&](auto& proposal){
			proposal.status = PROPOSAL_STATUS::ON_VOTE;
		});
	}

	//@abi action
	void wps_contract::rejectprop(account_name reviewer, uint64_t proposal_id, const string& reason) {
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
        eosio_assert(((*itr_proposal).status == PROPOSAL_STATUS::PENDING) || ((*itr_proposal).status == PROPOSAL_STATUS::ON_VOTE), "invalid proposal status");

		idx_index.modify(itr_proposal, (*itr_proposal).proposer, [&](auto& proposal){
			proposal.status = PROPOSAL_STATUS::REJECTED;
		});

		rejected_proposal_table rejected_proposals(_self, _self);
		rejected_proposals.emplace(reviewer, [&](auto& proposal){
			proposal = (*itr_proposal);
		});

		idx_index.erase(itr_proposal);
	}

	// @abi action
	void wps_contract::checkvote(account_name reviewer, uint64_t proposal_id) {
		require_auth(reviewer);

		reviewer_table reviewers(_self, _self);
		auto itr = reviewers.find(reviewer);
		eosio_assert(itr != reviewers.end(), "Account not found in reviewers table");

		proposal_table proposals(_self, _self);
		auto idx_index = proposals.get_index<N(idx)>();
		auto itr_proposal = idx_index.find(proposal_id);
		eosio_assert(itr_proposal != idx_index.end(), "Proposal not found in proposal table");
		eosio_assert((*itr_proposal).committee == (*itr).committee, "Reviewer is not part of this proposal's responsible committee");
		eosio_assert((*itr_proposal).status == PROPOSAL_STATUS::ON_VOTE, "Proposal's status is not ON_VOTE");

		idx_index.modify(itr_proposal, 0, [&](auto& proposal){
			proposal.status = PROPOSAL_STATUS::CHECK_VOTE;
		});
	}

	// @abi action
	void wps_contract::approve(account_name reviewer, uint64_t proposal_id) {
		require_auth(reviewer);

		reviewer_table reviewers(_self, _self);
		auto itr = reviewers.find(reviewer);
		eosio_assert(itr != reviewers.end(), "Account not found in reviewers table");

		proposal_table proposals(_self, _self);
		auto idx_index = proposals.get_index<N(idx)>();
		auto itr_proposal = idx_index.find(proposal_id);
		eosio_assert(itr_proposal != idx_index.end(), "Proposal not found in proposal table");
		eosio_assert((*itr_proposal).committee==(*itr).committee, "Reviewer is not part of this proposal's responsible committee");
		eosio_assert((*itr_proposal).status == PROPOSAL_STATUS::CHECKED_VOTE, "Proposal::status is not PROPOSAL_STATUS::CHECKED_VOTE");

		idx_index.modify(itr_proposal, 0, [&](auto& proposal){
			proposal.fund_start_time = now();
			proposal.status = PROPOSAL_STATUS::APPROVED;
		});
	}

	// @abi action
	void wps_contract::rmvreject(account_name reviewer, uint64_t proposal_id){
		require_auth(reviewer);

		reviewer_table reviewers(_self, _self);
		auto itr = reviewers.find(reviewer);
		eosio_assert(itr != reviewers.end(), "Account not found in reviewers table");

		rejected_proposal_table rejected_proposals(_self, _self);
		auto idx_index = rejected_proposals.get_index<N(idx)>();
		auto itr_proposal = idx_index.find(proposal_id);
		eosio_assert(itr_proposal != idx_index.end(), "Proposal not found in rejected proposal table");
		eosio_assert((*itr_proposal).status == PROPOSAL_STATUS::REJECTED, "Proposal::status is not PROPOSAL_STATUS::REJECTED");
		eosio_assert((*itr_proposal).committee==(*itr).committee, "Reviewer is not part of this proposal's responsible committee");

		idx_index.erase(itr_proposal);
	}

	// @abi action
	void wps_contract::rmvcompleted(account_name reviewer, uint64_t proposal_id){
		require_auth(reviewer);

		reviewer_table reviewers(_self, _self);
		auto itr = reviewers.find(reviewer);
		eosio_assert(itr != reviewers.end(), "Account not found in reviewers table");

		finished_proposal_table finished_proposals(_self, _self);
		auto idx_index = finished_proposals.get_index<N(idx)>();
		auto itr_proposal = idx_index.find(proposal_id);
		eosio_assert(itr_proposal != idx_index.end(), "Proposal not found in completed proposals table");
		eosio_assert((*itr_proposal).status == PROPOSAL_STATUS::COMPLETED, "Proposal::status is not PROPOSAL_STATUS::COMPLETED");
		eosio_assert((*itr_proposal).committee==(*itr).committee, "Reviewer is not part of this proposal's responsible committee");

		idx_index.erase(itr_proposal);
	}

} // eosiowps
