'use strict';

/*
    struct PROPOSAL_STATUS {
        const static uint8_t PENDING = 1;
        const static uint8_t REJECTED = 2;
        const static uint8_t ON_VOTE = 3;
        const static uint8_t FINISHED_VOTING = 4;
        const static uint8_t CHECK_VOTE = 5;     // check count of votes
        const static uint8_t CHECKED_VOTE = 6;   // checked count of votes by platform
        const static uint8_t APPROVED = 7;       // approve
        const static uint8_t COMPLETED = 8;
    };
*/

module.exports = exports = function(mongoose, conn) {
    const Schema = mongoose.Schema,
        schema = new Schema({
            proposer : {type : String, index : true},       // proposer
            proposal_id : {type : Number, index : true},    // proposer id
            committee : {type : String, index : true},      // committee
            category : {type : String},                     // category
            subcategory : {type : Number},                  // subcategory
            title : {type : String},                        // title
            summary : {type : String},                      // summary
            project_img_url : {type : String},              // project image or video url
            description : {type : String},                  // overview
            roadmap : {type : String},                      // roadmap
            duration : {type : Number},                     // duration
            members : [String],                             // linkedin
            funding_goal : {type : String},                 // EOS
            total_votes : {type : Number, default : 0},     // total votes
            agree_votes : {type : Number, default : 0},     // agree votes
            disagree_votes : {type : Number, default : 0},  // disagree votes
            status : {type : Number, index : true},         // status
            vote_start_time : {type : Number},              // time when voting starts (seconds)
            fund_start_time : {type : Number},              // time when funding starts (seconds)
            iteration_of_funding : {type : Number}          // number of iteration
        });
    return conn.model('LibProposal', schema);
};
