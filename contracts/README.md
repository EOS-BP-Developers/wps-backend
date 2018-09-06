# eosio.wps
## Version : 1.2.0

The design of the EOSIO blockchain calls for a number of smart contracts that are run at a privileged permission level in order to support functions such as working proposal.  These smart contracts are referred to as the wps contract.

Dependencies:
* [eosio v1.2.x](https://github.com/eosio/eos)
* [eosio.cdt v1.1.x](https://github.com/eosio/eosio.cdt)

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

Description: Accept a proposal with PENDING status. Change its status to ON VOTE. All fields required.

#### rejectprop

Required authority: Reviewer

Parameters: `account_name reviewer, uint64_t proposal_id, const string& reason`

Description: Reject a proposal with PENDING status. Change its status to REJECTED. Move proposal to the rejected proposals table. All fields required.

#### checkvote

Required authority: Reviewer

Parameters: `account_name reviewer, uint64_t proposal_id`

Description: Request the database to verify that the proposal with FINISHED VOTING status has passed the vote threshold. Change its status to CHECK VOTES. All fields required.

#### commitvote

Required authority: Committee (oversight)

Parameters: `account_name watchman, uint64_t proposal_id, uint64_t total_votes, uint64_t agree_votes, uint64_t disagree_votes`

Description: Oversight automatically checks proposals with the CHECK VOTES status using the database and verifies that the proposal passed the vote threshold through this action. The proposal status changes to CHECKED VOTES. All fields required.

#### rollbackvote

Required authority: Committee (oversight)

Parameters: `account_name watchman, uint64_t proposal_id, uint64_t total_votes, uint64_t agree_votes, uint64_t disagree_votes`

Description: Oversight automatically checks proposals with the CHECK VOTES status using the database. If the vote threshold hasn't been met, it rolls the status of the proposal back to ON VOTE. All fields required.

#### approve

Required authority: Reviewer

Parameters: `account_name reviewer, uint64_t proposal_id`

Description: Approve funding for proposals with the CHECKED VOTES status. Proposal status changes to APPROVED. All fields required.

#### claimfunds

Required authority: Proposer

Parameters: `account_name account, uint64_t proposal_id`

Description: Claim funding for a proposal with the APPROVED status. The proposer can claim a portion of the funds for each iteration of the project's duration. When all iterations have been completed, the proposal status changes to COMPLETED. It is then transferred to the completed proposals table. All fields required.

#### rmvreject

Required authority: Reviewer

Parameters: `account_name reviewer, uint64_t proposal_id`

Description: Clear a proposal on the rejected proposals table when it is no longer needed there.

#### rmvcompleted

Required authority: Reviewer

Parameters: `account_name reviewer, uint64_t proposal_id`

Description: Clear a proposal on the completed proposals table when it is no longer needed there.

#### vote

Required authority: Account owner

Parameters: `account_name voter, uint64_t proposal_id, bool is_agree`

Description: Vote for a proposal. The voter can choose to vote yes (true) or no (false) for a given proposal. In the database, the account's stake weight is counted. The account is added to the votings table. All fields required.

#### unvote

Required authority: Account owner

Parameters: `account_name voter, uint64_t proposal_id`

Descripton: Unvote for a proposal. The account is removed from the votings table. All fields required.

#### rejectfund

Required authority: Committee (oversight)

Parameters: `account_name committeeman, uint64_t proposal_id, const string& reason`

Description: Reject a proposal with APPROVED status being funded. The proposal is transferred to the rejected proposals table. All fiels required.

#### checkexpire

Required authority: None

Parameters: `account_name watchman, uint64_t proposal_id`

Description: Check whether a proposal has gone over the voting duration (default 30 days). If the duration has expired, the proposal is transferred to the rejected proposals table. All fields required.


## Tables

You can find information on the tables using `cleos`:
```console
cleos get table <contract account> <contract account> <table name>
```

On mainnet, since the contract account will be `eosio.wps`:
```console
cleos get table eosio.wps eosio.wps <table name>
```

#### proposals

Description: Table of ongoing proposals. Indexed by proposer account name and proposal id.

Code: `_self`

Scope: `_self`

#### rejectedpros

Description: Table of rejected proposals. Indexed by proposer account name and proposal id.

Code: `_self`

Scope: `_self`

#### finishedpros

Description: Table of completed proposals. Indexed by proposer account name and proposal id.

Code: `_self`

Scope: `_self`

#### proposers

Description: Table of proposers. Indexed by account name.

Code: `_self`

Scope: `_self`

#### reviewers

Description: Table of reviewers. Indexed by account name.

Code: `_self`

Scope: `_self`

#### committees

Description: Table of committees. Indexed by account name.

Code: `_self`

Scope: `_self`

#### votings

Description: Table of votes. Indexed by proposal id.

Code: `_self`

Scope: `_self`

#### wpsglobal

Description: Table of WPS global environment variables. This is a singleton, and not a multi index table.
