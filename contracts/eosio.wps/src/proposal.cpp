// #include <eosio.wps/eosio.wps.hpp>
#include <eosio.wps/proposal.hpp>

// extern struct permission_level;
// extern void require_auth(const permission_level& level);

namespace eosiowps {
	// @abi action
	void wps_contract::regproposal(account_name owner,
        uint16_t category,
        uint16_t subcategory,
        const string& title,
        const string& subtitle,
        const string& project_img_url,
        const string& project_overview,
        const string& financial_roadmap,
        const vector<string>& members,
		const asset& funding_goal,
		uint16_t duration
    ) {
		// authority of the user's account is required
		require_auth(owner);

		//verify that the inputs are not too short
		eosio_assert(category > 0, "category should be more than 0");
		eosio_assert(subcategory > 0, "subcategory should be more than 0");
		eosio_assert(title.size() > 0, "title should be more than 0 characters long");
		eosio_assert(subtitle.size() > 0, "subtitle should be more than 0 characters long");
		eosio_assert(project_img_url.size() > 0, "URL should be more than 0 characters long");
		eosio_assert(project_overview.size() > 4, "project_overview should be more than 0 characters long");
		eosio_assert(financial_roadmap.size() > 0, "financial_roadmap should be more than 0 characters long");
		eosio_assert(members.size() > 0, "member should be more than 0");

		//verify that the inputs aren't too long
		eosio_assert(title.size() < 256, "title should be shorter than 128 characters.");
		eosio_assert(subtitle.size() < 256, "subtitle should be shorter than 128 characters.");
		eosio_assert(project_img_url.size() < 128, "URL should be shorter than 128 characters.");
		eosio_assert(project_overview.size() < 1024, "project_overview should be shorter than 64 characters.");
		eosio_assert(financial_roadmap.size() < 256, "financial_roadmap should be shorter than 64 characters.");
		eosio_assert(members.size() < 50, "members should be shortter than shorter than 100.");

		// creates the proposal table if there isn't one already
		uint64_t id = 1;
		proposal_table proposals(_self, _self);
		auto itr = proposals.find(owner);
		// verify that the account doesn't already exist in the table
		eosio_assert(itr == proposals.end(), "This account has already been registered as a proposal");

		auto idIndex = proposals.get_index<N(idx)>();
		auto revItr = idIndex.rbegin();
		if (revItr != idIndex.rend()) {
			id = revItr->id + 1;
		}

		// add to the table
		// storage is billed to the contract account
		proposals.emplace(owner, [&](auto& proposal) {
			proposal.owner = owner;
			proposal.category = category;
			proposal.subcategory = subcategory;
			proposal.title = title;
			proposal.subtitle = subtitle;
			proposal.project_img_url = project_img_url;
			proposal.project_overview = project_overview;
			proposal.financial_loadmap = financial_loadmap;
			proposal.members = members;
			proposal.funding_goal = funding_goal;
			proposal.id = id;
			proposal.duration = duration;
		});
	}

	//@abi action
	void wps_contract::editproposal(account_name owner,
        uint16_t category,
        uint16_t subcategory,
        const string& title,
        const string& subtitle,
        const string& project_img_url,
        const string& project_overview,
        const string& financial_loadmap,
        const vector<string>& members,
		const asset& funding_goal,
		uint16_t duration
    ) {
		// authority of the user's account is required
		require_auth(owner);

		//verify that the inputs are not too short
		eosio_assert(category > 0, "category should be more than 0");
		eosio_assert(subcategory > 0, "subcategory should be more than 0");
		eosio_assert(title.size() > 0, "title should be more than 0 characters long");
		eosio_assert(subtitle.size() > 0, "subtitle should be more than 0 characters long");
		eosio_assert(project_img_url.size() > 0, "URL should be more than 0 characters long");
		eosio_assert(project_overview.size() > 4, "project_overview should be more than 0 characters long");
		eosio_assert(financial_loadmap.size() > 0, "financial_loadmap should be more than 0 characters long");
		eosio_assert(members.size() > 0, "member should be more than 0");

		//verify that the inputs aren't too long
		eosio_assert(title.size() < 128, "title should be shorter than 128 characters.");
		eosio_assert(subtitle.size() < 128, "subtitle should be shorter than 128 characters.");
		eosio_assert(project_img_url.size() < 128, "URL should be shorter than 128 characters.");
		eosio_assert(project_overview.size() < 64, "project_overview should be shorter than 64 characters.");
		eosio_assert(financial_loadmap.size() < 64, "financial_loadmap should be shorter than 64 characters.");
		eosio_assert(members.size() < 100, "members should be shortter than shorter than 100.");

		proposal_table proposals(_self, _self);

		auto itr = proposals.find(owner);
		// verify that the account doesn't already exist in the table
		eosio_assert(itr != proposals.end(), "Account not found in proposal table");

		// modify value in the table
		proposals.modify(itr, owner, [&](auto& proposal){
			proposal.owner = owner;
			proposal.category = category;
			proposal.subcategory = subcategory;
			proposal.title = title;
			proposal.subtitle = subtitle;
			proposal.project_img_url = project_img_url;
			proposal.project_overview = project_overview;
			proposal.financial_loadmap = financial_loadmap;
			proposal.members = members;
			proposal.funding_goal = funding_goal;
			proposal.duration = duration;
		});
	}

	//@abi action
	void wps_contract::rmvproposal(const account_name owner){
		// needs authority of the proposers's account
		require_auth(owner);

		proposal_table proposals(_self, _self);

		// verify that the account already exists in the proposer table
		auto itr = proposals.find(owner);
		eosio_assert(itr != proposals.end(), "Account not found in proposal table");

		proposals.erase(itr);
	}
} // eosiowps
