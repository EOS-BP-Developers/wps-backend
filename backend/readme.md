# wps backend server
## Version : 1.0.0

## Server Info
### required
    mongodb
    redis
    nodejs
### development server info
    http://http://wps-test.hkeos.com

    ex)
    http://wps-test.hkeos.com/api/1/summary

### production server info
    Coming soon

## start
### npm install
    npm install
    npm install -g pm2

### edit nodeos, mongo, redis environment
    vi config/developer.json
    vi tools/create_wps_info.js
    NODE_ENV=developer NODE_PATH=. node tools/create_wps_info.js

### start pm2
    pm2 start deploy/{eco_xxx.json}

### stop pm2
    pm2 kill

## API
    The format of the returned value is json.

### stats
    GET /api/1/summary
    Returns summary object
    return
        summary

### user
    POST /api/1/users
    This method create user account.

    body parameter
        eos_account (string max 12, required) : eos account,
        nick (string max 32, required) : nick name,
        img_url (string max 128): profile image url,
        bio (string max 256): bio,
        location (string max 50): location,
        telegram (string max 64): telegram username,
        website (string max 128): website url,
        linkedin (string max 128): linkedin account
    retun
        user : {
        _id (string) : mongo Object Id,
        eos_account (string) : eos account,
        nick (string) : nick name,
        img_url (string): profile image url,
        bio (string): bio,
        location (string): location,
        telegram (string): telegram username,
        website (string): website url,
        linkedin (string): linkedin account
    }

    ex)
        eos_account : 'useraaaaaaam',
        nick : 'yepps',
        img_url : 'https://i.amz.mshcdn.com/3NbrfEiECotKyhcUhgPJHbrL7zM=/950x534/filters:quality(90)/2014%2F06%2F02%2Fc0%2Fzuckheadsho.a33d0.jpg',
        bio : 'Rodney Erickson is a content marketing professional at HubSpot, an inbound marketing and sales platform that helps companies attract visitors, convert leads, and close customers. Previously, Rodney worked as a marketing manager for a tech software startup.',
        location : 'USA, WA',
        telegram : 'yepp4you',
        website : 'www.google.com',
        linkedin : 'yepp4you'

    PUT /api/1/users/{eos_account}
    This method update user account.
    params
        eos_account : eos account
    body parameter
        nick (string max 32, required) : nick name,
        img_url (string max 128): profile image url,
        bio (string max 256): bio,
        location (string max 50): location,
        telegram (string max 64): telegram username,
        website (string max 128): website url,
        linkedin (string max 128): linkedIn account
    return
        user

    GET /api/1/users/{eos_account}
    Returns user object.
    params :
        eos_accout : eos account
    return
        user



### proposal
    GET /api/1/committees/{committee}/proposals/orders/{order}
    Returns proposal object list order by `order parameter`
    params :
        committee (string): eos acount of committee
        order (string): 'hot' or 'latest'
    return
        proposals : [proposal object]

    GET /api/1/committees/{committee}/proposals/{proposal_id}
    Returns proposal object
    params
        committee (string) : eos committee acount name
        proposal_id (number) : proposal's id
    return
        proposal

### proposer
    GET /api/1/proposers/{proposer}/proposals
    Returns proposal object list of proposer
    params
        proposer : eos account of proposer
    return
        proposals : [proposal object]

### committee
    GET /api/1/committees
    Returns committee object list (you can get category list)
    return
        committees : [committee object]

### object
    summary
    {
        _id (string) : mongo object id
        block_id (string) : block id
        block_num (number) : block number
        funded_proposals (number) : number of funded proposal
        ongoing_proposals (number) : number of ongoing proposal
        total_proposals (number ) : number of total proposal
        total_voters (number) : number of total voter
    }

    committee
    {
        _id (string) : mongo object id
        committeeman (string) : eos account of committee
        category (string) : category name of committee (ex. emergency)
        is_oversight (boolean) : if true, have oversight right, otherwise not oversight right
    }

    user
    {
        _id (string) : mongo ObjectId
        eos_account (string) : eos account
        nick (string) : nick name
        img_url (string) : profile image url
        bio (string) : bio
        location (string) : location
        telegram (string) : telegram username
        website (string) : website url
        linkedin (string): linkedin account
    }

    proposal
    {
        _id (string) : mongo ObjectId
        proposer (string) : proposer
        proposal_id (number) : proposer id
        committee (string) : committee
        category (string) : category
        subcategory (number) : subcategory
        title (string) : title
        summary (string) : summary
        project_img_url (string) : project image or video url
        description (string) : overview
        roadmap (string) : roadmap
        duration (number) : duration
        members ([string]) : linkedin
        funding_goal (string) : EOS
        total_votes (number) : total votes
        agree_votes (number) :, agree votes
        disagree_votes (number) :  disagree votes
        status (number) : status (1 : pending, 2 : reject, 3 : on vote, 4 : check vote, 5 : checked vote, 6 : approved, 7 : completed)
        vote_start_time (number) : time when voting starts (seconds)
        fund_start_time (number) : time when funding starts (seconds)
        iteration_of_funding (number) : number of funding iteration
    }
