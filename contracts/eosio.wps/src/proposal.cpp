// #include <eosio.wps/eosio.wps.hpp>
#include <eosio.wps/proposal.hpp>
#include <eosio.wps/proposer.hpp>
#include <eosio.wps/committee.hpp>

// extern struct permission_level;
// extern void require_auth(const permission_level& level);

namespace eosiowps {
	// @abi action
	void wps_contract::regproposal(
		account_name proposer,
        account_name committee,
        uint16_t subcategory,
        const string& title,
        const string& subtitle,
        const string& project_img_url,
        const string& project_overview,
        const string& financial_roadmap,
        const vector<string>& members,
		const asset& funding_goal,
    ) {
		// authority of the user's account is required
		require_auth(proposer);

		// verify that the committee account exists
		eosio_assert(is_account(committee), "committee account doesn't exist");

		//verify that the inputs are not too short
		//subcategory is not required
		//eosio_assert(subcategory > 0, "subcategory should be an integer greater than 0");
		eosio_assert(title.size() > 0, "title should be more than 0 characters long");
		eosio_assert(subtitle.size() > 0, "subtitle should be more than 0 characters long");
		eosio_assert(project_img_url.size() > 0, "URL should be more than 0 characters long");
		eosio_assert(project_overview.size() > 0, "project_overview should be more than 0 characters long");
		eosio_assert(financial_roadmap.size() > 0, "financial_roadmap should be more than 0 characters long");
		eosio_assert(members.size() > 0, "member should be more than 0");
		eosio_assert(duration > 0, "duration must be greater than 0 days");

		//verify that the inputs aren't too long
		eosio_assert(subcategory < 10, "invalid sub-category");
		eosio_assert(title.size() < 256, "title should be shorter than 256 characters.");
		eosio_assert(subtitle.size() < 256, "subtitle should be shorter than 256 characters.");
		eosio_assert(project_img_url.size() < 128, "URL should be shorter than 128 characters.");
		eosio_assert(project_overview.size() < 1024, "project_overview should be shorter than 1024 characters.");
		eosio_assert(financial_roadmap.size() < 256, "financial_roadmap should be shorter than 256 characters.");
		eosio_assert(members.size() < 50, "members should be shorter than 50 characters.");
		eosio_assert(duration <= m_wps_env.duration_of_voting, "duration should be less than duration_of_voting days.");

		//initializing the proposer table
		proposer_table proposers(_self, _self);

		auto itr = proposers.find(proposer);
		// verify that the account is a registered proposer
		eosio_assert(itr != proposers.end(), "This account is not a registered proposer");

		// creates the proposal table if there isn't one already
		proposal_table proposals(_self, _self);
		auto proposal_itr = proposals.find(proposer);
		// verify that the account doesn't already exist in the table
		eosio_assert(proposal_itr == proposals.end(), "This account has already registered a proposal");

		//creates the committee table if it doesn't exist already
		committee_table committees(_self, _self);

		auto committee_itr = committees.find(committee);
		// verify that the committee is on committee table
		eosio_assert(committee_itr != committees.end(), "Account not found in committee table");

		m_wps_env = m_wps_env_global.exists() ? m_wps_env_global.get() : wps_env();
		m_wps_env.proposal_current_index += 1;
		m_wps_env_global.set( m_wps_env, _self );

		// add to the table
		// storage is billed to the contract account
		proposals.emplace(proposer, [&](auto& proposal) {
			proposal.proposer = proposer;
			proposal.committee = committee;
			proposal.category = (*committee_itr).category;
			proposal.subcategory = subcategory;
			proposal.title = title;
			proposal.subtitle = subtitle;
			proposal.project_img_url = project_img_url;
			proposal.project_overview = project_overview;
			proposal.financial_roadmap = financial_roadmap;
			proposal.members = members;
			proposal.funding_goal = funding_goal;
			proposal.id = m_wps_env.proposal_current_index;
			proposal.status = PROPOSAL_STATUS::PENDING; 		//initialize status to pending
			proposal.iteration_of_funding = 0;
			proposal.fund_start_time = 0;
		});
	}

	//@abi action
	void wps_contract::editproposal(
		account_name proposer,
        account_name committee,
        uint16_t subcategory,
        const string& title,
        const string& subtitle,
        const string& project_img_url,
        const string& project_overview,
        const string& financial_roadmap,
        const vector<string>& members,
		const asset& funding_goal,
    ) {
		// authority of the user's account is required
		require_auth(proposer);

		// verify that the committee account exists
		eosio_assert(is_account(committee), "committee account doesn't exist");

		//verify that the inputs are not too short
		//subcategory is not required
		//eosio_assert(subcategory > 0, "subcategory should be an integer greater than 0");
		eosio_assert(title.size() > 0, "title should be more than 0 characters long");
		eosio_assert(subtitle.size() > 0, "subtitle should be more than 0 characters long");
		eosio_assert(project_img_url.size() > 0, "URL should be more than 0 characters long");
		eosio_assert(project_overview.size() > 0, "project_overview should be more than 0 characters long");
		eosio_assert(financial_roadmap.size() > 0, "financial_roadmap should be more than 0 characters long");
		eosio_assert(members.size() > 0, "member should be more than 0");
		eosio_assert(duration > 0, "duration must be greater than 0 days");

		//verify that the inputs aren't too long
		eosio_assert(subcategory < 10, "invalid sub-category");
		eosio_assert(title.size() < 256, "title should be shorter than 256 characters.");
		eosio_assert(subtitle.size() < 256, "subtitle should be shorter than 256 characters.");
		eosio_assert(project_img_url.size() < 128, "URL should be shorter than 128 characters.");
		eosio_assert(project_overview.size() < 1024, "project_overview should be shorter than 1024 characters.");
		eosio_assert(financial_roadmap.size() < 256, "financial_roadmap should be shorter than 256 characters.");
		eosio_assert(members.size() < 50, "members should be shorter than 50 characters.");
        eosio_assert(duration <= m_wps_env.duration_of_voting, "duration should be less than duration_of_voting days.");

		//initializing the proposer table
		proposer_table proposers(_self, _self);

		auto itr = proposers.find(proposer);
		// verify that the account is a registered proposer
		eosio_assert(itr != proposers.end(), "This account is not a registered proposer");

		proposal_table proposals(_self, _self);

		auto proposal_itr = proposals.find(proposer);
		// verify that the account already exists in the proposals table
		eosio_assert(proposal_itr != proposals.end(), "Account not found in proposal table");

		//creates the committee table if it doesn't exist already
		committee_table committees(_self, _self);

		auto committee_itr = committees.find(committee);
		// verify that the committee is on committee table
		eosio_assert(committee_itr != committees.end(), "Account not found in committee table");

		// modify value in the table
		proposals.modify(proposal_itr, 0, [&](auto& proposal){
			proposal.proposer = proposer;
			proposal.committee = committee;
			proposal.category = (*committee_itr).category;
			proposal.subcategory = subcategory;
			proposal.title = title;
			proposal.subtitle = subtitle;
			proposal.project_img_url = project_img_url;
			proposal.project_overview = project_overview;
			proposal.financial_roadmap = financial_roadmap;
			proposal.members = members;
			proposal.funding_goal = funding_goal;
		});
	}

	//@abi action
	void wps_contract::rmvproposal(const account_name proposer){
		// needs authority of the proposers's account
		require_auth(proposer);

		proposal_table proposals(_self, _self);

		// verify that the account already exists in the proposer table
		auto itr = proposals.find(proposer);
		eosio_assert(itr != proposals.end(), "Account not found in proposal table");

		proposals.erase(itr);
	}
} // eosiowps
