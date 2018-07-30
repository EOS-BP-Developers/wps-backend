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

		uint64_t voting_delta = 0;
		voter_table	voters(_self, _self);
		auto itr = voters.find(voter);
		if (itr == voters.end()) {
			voters.emplace(_self, [&](auto& _voter_info) {
				_voter_info.owner = voter;
				_voter_info.proposals.emplace(proposal_id);
			});
			voting_delta = 1;
		} else {
			voters.modify(itr, _self, [&](auto& _voter_info) {
				if (_voter_info.proposals.find(proposal_id) == _voter_info.proposals.end()) {
					voting_delta = 1;
					_voter_info.proposals.emplace(proposal_id);
				}
			});
		}

		idx_index.modify(itr_proposal, (*itr_proposal).owner, [&](auto& proposal) {
			proposal.total_votes += voting_delta;
			if (proposal.total_votes >= m_wps_info.lower_bound_total_voting) {
				proposal.status = proposal_status::VOTED;
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

		uint64_t voting_delta = 0;
		voter_table	voters(_self, _self);
		auto itr = voters.find(voter);
		eosio_assert(itr != voters.end(), "Proposal not found in voter table");

		voters.modify(itr, _self, [&](auto& _voter_info) {
			if (_voter_info.proposals.find(proposal_id) != _voter_info.proposals.end()) {
				voting_delta = 1;
				_voter_info.proposals.erase(proposal_id);
			}
		});

		idx_index.modify(itr_proposal, (*itr_proposal).owner, [&](auto& proposal) {
			proposal.total_votes -= voting_delta;
		});
	}

} // eosiowps
