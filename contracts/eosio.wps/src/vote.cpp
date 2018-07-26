// #include <eosio.wps/eosio.wps.hpp>
#include <eosio.wps/reviewer.hpp>
#include <eosio.wps/proposal.hpp>

// extern struct permission_level;
// extern void require_auth(const permission_level& level);

namespace eosiowps {
	// @abi action
	void wps_contract::vote(account_name voter, uint64_t proposal_id) {
		require_auth(voter);

		proposal_table proposals(_self, _self);
		auto idx_index = proposals.get_index<N(idx)>();
		auto itr_proposal = idx_index.find(proposal_id);
		eosio_assert(itr_proposal != idx_index.end(), "Proposal not found in proposal table");
		eosio_assert((*itr_proposal).status == proposal_status::ON_VOTE, "Proposal::status is not proposal_status::ON_VOTE");

		wps_info _wps_info = this->_wps_info_global.get();

		uint64_t voting_delta = 0;
		voter_table	voters(_self, _self);
		auto itr = voters.find(voter);
		if (itr == voters.end()) {
			voters.emplace(_self, [&](vote_info) {
				vote_info.owner = voter;
				vote_info.proposals.emplace(proposal_id);
			});
			voting_delta = 1;
		} else {
			voters.modify(itr, _self, [&](vote_info) {
				if (vote_info.proposals.find(proposal_id) == vote_info.proposals.end()) {
					voting_delta = 1;
					vote_info.proposals.emplace(proposal_id);
				}
			});
		}

		idx_index.modify(itr_proposal, (*itr_proposal).owne, [&](proposal) {
			proposal.total_votes += voting_delta;
			if (proposal.total_votes >= _wps_info.lower_bound_total_voting) {
				proposal.status = proposal_status::FUNDED;
			}
		});
	}

	// @abi action
	void wps_contract::unvote(account_name voter, uint64_t proposal_id) {
		require_auth(voter);

		proposal_table proposals(_self, _self);
		auto idx_index = proposals.get_index<N(idx)>();
		auto itr_proposal = idx_index.find(proposal_id);
		eosio_assert(itr_proposal != idx_index.end(), "Proposal not found in proposal table");
		eosio_assert((*itr_proposal).status == proposal_status::ON_VOTE, "Proposal::status is not proposal_status::ON_VOTE");

		wps_info _wps_info = this->_wps_info_global.get();

		uint64_t voting_delta = 0;
		voter_table	voters(_self, _self);
		auto itr = voters.find(voter);
		eosio_assert(itr != voters.end(), "Proposal not found in voter table");

		voters.modify(itr, _self, [&](vote_info) {
			if (vote_info.proposals.find(proposal_id) != vote_info.proposals.end()) {
				voting_delta = 1;
				vote_info.proposals.remove(proposal_id);
			});

		idx_index.modify(itr_proposal, (*itr_proposal).owne, [&](proposal) {
			proposal.total_votes -= voting_delta;
		});
	}

} // eosiowps
