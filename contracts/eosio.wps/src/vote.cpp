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
		eosio_assert((*itr_proposal).status == PROPOSAL_STATUS::ON_VOTE, "Proposal::status is not PROPOSAL_STATUS::ON_VOTE");

		m_wps_env = m_wps_env_global.exists() ? m_wps_env_global.get() : wps_env();
		auto current_time = now();
		//check for vote time expiry
		if(current_time - (*itr_proposal).vote_start_time > duration_of_voting_seconds){
			rejected_proposal_table rejected_proposals(_self, _self);

			//add to the table
			rejected_proposals.emplace((*itr_proposal).proposer, [&](auto& _proposal){
				_proposal = std::move(*itr_proposal);
				_proposal.status = PROPOSAL_STATUS::REJECTED;
			});

			idx_index.erase(itr_proposal);
			//move tables
			eosio_assert(current_time - (*itr_proposal).vote_start_time < duration_of_voting_seconds, "The funding period for this proposal has expired.");
		}

		uint64_t voting_delta = 0;

		voter_table	voters(_self, _self);

		//initialize vote_start_time
		if(/*table is empty*/){
			idx_index.modify(itr_proposal, 0, [&](auto& _proposal){
				_proposal.vote_start_time = current_time;
			});
		}

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
	}

	// @abi action
	void wps_contract::unvote(account_name voter, uint64_t proposal_id) {
		require_auth(voter);

		proposal_table proposals(_self, _self);
		auto idx_index = proposals.get_index<N(idx)>();
		auto itr_proposal = idx_index.find(proposal_id);
		eosio_assert(itr_proposal != idx_index.end(), "Proposal not found in proposal table");
		eosio_assert((*itr_proposal).status == PROPOSAL_STATUS::ON_VOTE, "Proposal::status is not proposal_status::PROPOSAL_STATUS");

		m_wps_env = m_wps_env_global.exists() ? m_wps_env_global.get() : wps_env();
		auto current_time = now();
		//check for vote time expiry
		if(current_time - (*itr_proposal).vote_start_time > duration_of_voting_seconds){
			rejected_proposal_table rejected_proposals(_self, _self);

			//add to the table
			rejected_proposals.emplace((*itr_proposal).proposer, [&](auto& _proposal){
				_proposal = std::move(*itr_proposal);
				_proposal.status = PROPOSAL_STATUS::REJECTED;
			});

			idx_index.erase(itr_proposal);
			//move tables
			eosio_assert(current_time - (*itr_proposal).vote_start_time < duration_of_voting_seconds, "The funding period for this proposal has expired.");
		}

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

		idx_index.modify(itr_proposal, 0, [&](auto& proposal) {
			proposal.total_votes -= voting_delta;
		});
	}

} // eosiowps
