#include <eosio.wps/reviewer.hpp>
#include <eosio.wps/proposal.hpp>
#include <eosio.wps/committee.hpp>

namespace eosiowps {

    // @abi action
    void wps_contract::setwpsenv(uint32_t total_voting_boundary, uint32_t voting_duration_day, uint32_t payment_duration_day, uint32_t split_duration_day) {
        //registration of committee requires contract account permissions
        require_auth(_self);

        eosio_assert(total_voting_boundary >= 5, "total_voting_boundary should be more than equal 5 long");
        eosio_assert(voting_duration_day > 0, "voting_duration_day should be more than 0");
        eosio_assert(payment_duration_day > 0, "payment_duration_day should be more than 0");
        eosio_assert(split_duration_day > 0, "split_duration_day should be more than 0");

        m_wps_env = m_wps_env_global.exists() ? m_wps_env_global.get() : wps_env();

        m_wps_env.total_voting_boundary = total_voting_boundary;
        m_wps_env.voting_duration_day = voting_duration_day;
        m_wps_env.payment_duration_day = payment_duration_day;
        m_wps_env.split_duration_day = split_duration_day;

        m_wps_env_global.set( m_wps_env, _self );
    }

    // @abi action
    void wps_contract::regcommittee(account_name owner, const string& category, bool is_oversight) {
        //registration of committee requires contract account permissions
        require_auth(_self);

        //verify that the committee account exists
        eosio_assert(is_account(owner), "committee account doesn't exist");

        //verify that the size of the category string is not too long/short
        eosio_assert(category.size() > 0, "category should be more than 0 characters long");
        eosio_assert(category.size() < 64, "category should be less than 64 characters long");

        //creates the committee table if it doesn't exist already
        committee_table committees(_self, _self);

        auto itr = committees.find(owner);
        // verify that the account doesn't already exist in the table
        eosio_assert(itr == committees.end(), "This account has already been registered as a committee");

        //add to the table
        committees.emplace(owner, [&](auto& committee){
            committee.owner = owner;
            committee.category = category;
            committee.is_oversight = is_oversight;
        });
    }

    // @abi action
    void wps_contract::editcommittee(account_name owner, const string& category, bool is_oversight) {
        //editing committee info requires contract account permissions
        require_auth(_self);

        //verify that the committee account exists
        eosio_assert(is_account(owner), "committee account doesn't exist");

        //verify that the size of the category string is not too long/short
        eosio_assert(category.size() > 0, "category should be more than 0 characters long");
        eosio_assert(category.size() < 64, "category should be less than 64 characters long");

        //creates the committee table if it doesn't exist already
        committee_table committees(_self, _self);

        auto itr = committees.find(owner);
        // verify that the account doesn't already exist in the table
        eosio_assert(itr != committees.end(), "Account not found in committee table");

        //add to the table
        committees.modify(itr, owner, [&](auto& committee){
            committee.owner = owner;
            committee.category = category;
            committee.is_oversight = is_oversight;
        });
    }

    // @abi action
    void wps_contract::rmvcommittee(account_name owner) {
        require_auth(_self);

        eosio_assert(is_account(owner), "committee account doesn't exist");

        committee_table committees(_self, _self);
        auto itr = committees.find(owner);

        eosio_assert(itr != committees.end(), "Account not found in committee table");
        committees.erase( itr );
    }

    // @abi action
	void wps_contract::rejectfunding(account_name owner, uint64_t proposal_id, const string& reason){
		require_auth(owner);

		eosio_assert(reason.size() > 0, "must provide a brief reason");
		eosio_assert(reason.size() < 256, "reason is too long");

		committee_table committees(_self, _self);

		auto itr = committees.find(owner);
		// verify that the committee is on committee table
		eosio_assert(itr != committees.end(), "Account not found in committee table");

		approved_proposal_table approved_proposals(_self, _self);

		auto idx_index = approved_proposals.get_index<N(idx)>();
		auto itr_proposal = idx_index.find(proposal_id);
		eosio_assert(itr_proposal != idx_index.end(), "Proposal not found in approved proposal table");

		eosio_assert(((*itr_proposal).committee==(*itr).owner) || (*itr).is_oversight, "Committee is not associated with this proposal");
		eosio_assert((*itr_proposal).status == proposal_status::APPROVED, "Proposal::status is not proposal_status::APPROVED");

		rejected_proposal_table rejected_proposals(_self, _self);

		//add to the table
		approved_proposals.emplace((*itr_proposal).owner, [&](auto& proposal){
			proposal = std::move(*itr_proposal);
			proposal.status = proposal_status::REJECT;
		});

		idx_index.erase(itr_proposal);
	}
} //eosiowps