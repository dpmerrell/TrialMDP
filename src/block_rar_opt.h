// block_rar_opt.h
// (c) David Merrell 2020-08
//
// Class that represents a block RAR RCT design
// problem instance.
// 
// Important attributes:
//   * n_patients: the number of patients in the trial
//   * block_incr: an increment for block sizes 
//                 (all block sizes are multiples of this number)
//   * failure_cost: the cost of assigning a patient to the inferior treatment
//   * block_cost: the cost of running a block
//
// Important methods:
//   * solve():      perform the dynamic programming algorithm,
//                   obtaining an optimal policy governing the RCT.
//   * to_sqlite():  save the optimal policy to a SQLite database.

#ifndef _BLOCK_RAR_OPT_H
#define _BLOCK_RAR_OPT_H

#include "contingency_table.h"
#include "state_result.h"
#include "block_rar_table.h"
#include "state_iterator.h"
#include "action_iterator.h"
#include "transition_dist.h"
#include "terminal_reward.h"
#include "transition_reward.h"
#include <string>

class BlockRAROpt{

    private:
	// Data
        int n_patients;
        int block_incr;
	float failure_cost;
	float block_cost;
	
        BlockRARTable* results_table;
        StateIterator* state_iterator;
	ActionIterator* action_iterator;
        TransitionDist* transition_dist;
        TerminalReward* terminal_reward; 
        TransitionReward* transition_reward;

	// Private methods
	StateResult max_expected_reward(int cur_idx, ContingencyTable ct);

    public:

        // Constructor
	BlockRAROpt(int n_patients, int block_incr, float failure_cost, float block_cost,
                    float prior_a0, float prior_a1,
                    float prior_b0, float prior_b1, 
                    std::string transition_dist="beta_binom",
                    std::string transition_rwd="block_cost",
                    std::string terminal_rwd="wald_failure");

	void solve();

	void to_sqlite(char* db_fname, int chunk_size);

	// Destructor
	~BlockRAROpt();
};

#endif
