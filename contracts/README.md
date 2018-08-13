# eosio.wps
## Version : 1.0.0

The design of the EOSIO blockchain calls for a number of smart contracts that are run at a privileged permission level in order to support functions such as working proposal.  These smart contracts are referred to as the wps contract.

Dependencies:
* [eosio v1.1.4](https://github.com/eosio/eos/tree/v1.1.4)
* [eosio.wasmsdk v1.1.1](https://github.com/eosio/eosio.wasmsdk/tree/v1.1.1)

To build the contracts and the unit tests:
* First, ensure that your __eosio__ is compiled to the core symbol for the EOSIO blockchain that intend to deploy to.
* Second, make sure that you have ```sudo make install```ed __eosio__.
The naming convention is codeaccount::actionname followed by a list of paramters.

After build:
* The unit tests executable is placed in the _build/tests_ and is named __unit_test__.
* The contracts are built into a _bin/\<contract name\>_ folder in their respective directories.
* Finally, simply use __cleos__ to _set contract_ by pointing to the previously mentioned directory.

## Actions

Note: When the required authority is `_self`, it means that the authority of the contract account is required. When deployed onto the `eosio.wps` account on mainnet, this authority will be given to `eosio`, which is owned by `eosio.prods`. We also expect that most committees will be "owned" by its reviewers, where the committee has an `msig` permission set up and controlled by its members.

#### setwpsenv

Required authority: `_self`

Parameters: `uint32_t total_voting_percent, uint32_t duration_of_voting, uint32_t max_duration_of_funding, uint32_t total_iteration_of_funding`

Description: Sets up the global WPS parameters, which includes vote participation required, duration of voting for a proposal, maximum duration of a project, and the number of times the funding is divided and claimed (for security reasons). The default values proposed when the WPS is ratified will be 5, 30, 180, and 6, respectively.

#### regproposer

Required authority: Account owner

Parameters: `account_name account, const string& first_name, const string& last_name,
                            const string& img_url, const string& bio, const string& country, const string& telegram,
                            const string& website, const string& linkedin`

Description: Register an account as a proposer. All fields required. RAM is billed to the registrant's account. Account is added to the proposers table.

#### editproposer

Required authority: Account owner

Parameters: `account_name account, const string& first_name, const string& last_name,
                            const string& img_url, const string& bio, const string& country, const string& telegram,
                            const string& website, const string& linkedin`

Description: Edit proposer info. All fields required.

#### rmvproposer

Required authority: Account owner

Parameters: `account_name account`

Description: Remove account from the proposers table.

#### regproposal

Required authority: Proposer

Parameters: `account_name proposer,
                    account_name committee,
                    uint16_t subcategory,
                    const string& title,
                    const string& summary,
                    const string& project_img_url,
                    const string& description,
                    const string& roadmap,
                    uint64_t duration,
                    const vector<string>& members,
                    const asset& funding_goal`

Description: Register a proposal. Account must be on the proposers table. All fields required. RAM is billed to the proposer's account. Proposal is added to the proposals table. One proposer can register only one proposal at a time.

#### editproposal

Required authority: Proposer

Parameters: `account_name proposer,
                    account_name committee,
                    uint16_t subcategory,
                    const string& title,
                    const string& summary,
                    const string& project_img_url,
                    const string& description,
                    const string& roadmap,
                    uint64_t duration,
                    const vector<string>& members,
                    const asset& funding_goal`

Description: Edit proposal info. All fields required.

#### rmvproposal

Required authority: Proposer

Parameters: `account_name proposer`

Description: Delete proposal from the proposals table.

#### regcommittee

Required authority: `_self`

Parameters: `account_name committeeman, const string& category, bool is_oversight`

Description: Register a committee responsible for a certain category. The account is added to the committees table. RAM is billed to the contract's account. All fields are required. Oversight power is given to the oversight committee. Committees can only be registered using `eosio.wps` permissions.

#### edcommittee

Required authority: `_self`

Parameters: `account_name committeeman, const string& category, bool is_oversight`

Description: Edit committee information. All fields required.

#### rmvcommittee

Required authority: `_self`

Parameters: `account_name committeeman`

Description: Remove committee from the committees table.

#### regreviewer

Required authority: Committee

Parameters: `account_name committee, account_name reviewer, const string& first_name, const string& last_name`

Description: Register account as a reviewer. All fields required. RAM billed to committee account. Reviewer is added to reviewers table, with the committee that the account is associated with.

#### editreviewer

Required authority: Committee

Parameters: `account_name committee, account_name reviewer, const string& first_name, const string& last_name`

Description: Edit reviewer information. All fields required.

#### rmvreviewer

Required authority: Committee

Parameters: `account_name committee, const account_name reviewer`

Description: Remove reviewer from the reviewers table.

#### acceptprop

Required authority: Reviewer

Parameters: `account_name reviewer, uint64_t proposal_id`

#### rejectprop

Required authority: Reviewer

Parameters: `account_name reviewer, uint64_t proposal_id, const string& reason`

#### checkvote

Required authority: Reviewer

Parameters: `account_name reviewer, uint64_t proposal_id`

#### approve

Required authority: Reviewer

Parameters: `account_name reviewer, uint64_t proposal_id`

#### claimfunds

Required authority: Proposer

Parameters: `account_name account, uint64_t proposal_id`

#### rmvreject

Required authority: Reviewer

Parameters: `account_name reviewer, uint64_t proposal_id`

#### rmvcompleted

Required authority: Reviewer

Parameters: `account_name reviewer, uint64_t proposal_id`

#### vote

Required authority: Account owner

Parameters: `account_name voter, uint64_t proposal_id, bool is_agree`

#### unvote

Required authority: Account owner

Parameters: `account_name voter, uint64_t proposal_id`

#### rejectfund

Required authority: Committee (oversight)

Parameters: `account_name committeeman, uint64_t proposal_id, const string& reason`

#### commitvote

Required authority: Committee (oversight)

Parameters: `account_name watchman, uint64_t proposal_id`

#### rollbackvote

Required authority: Committee (oversight)

Parameters: `account_name watchman, uint64_t proposal_id`

#### checkexpire

Required authority: None

Parameters: `account_name watchman, uint64_t proposal_id`
