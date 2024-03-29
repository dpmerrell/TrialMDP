// trial_mdp.h
// (c) David Merrell 2020-08
//
// Class that represents a multistage adaptive trial design
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

#ifndef _TRIAL_MDP_H
#define _TRIAL_MDP_H

#include "result_interpreter.h"
#include "contingency_table.h"
#include "state_result.h"
#include "trial_mdp_table.h"
#include "state_iterator.h"
#include "action_iterator.h"
#include "transition_dist.h"
#include "terminal_rule.h"
#include <string>

class TrialMDP{

    private:
	// Data
        int n_patients;
        int block_incr;
	float failure_cost;
	float block_cost;

        int n_attr;
	
        TrialMDPTable* results_table;
        StateIterator* state_iterator;
	ActionIterator* action_iterator;
        TransitionDist* transition_dist;
        TerminalRule* terminal_rule; 
        
        ResultInterpreter result_interpreter; 

	// Private methods
	StateResult max_expected_reward(int cur_idx, ContingencyTable ct);

    public:

        // Constructor
	TrialMDP(int n_patients, float failure_cost, float block_cost,
                    int min_size, int block_incr, 
                    float prior_a0, float prior_a1,
                    float prior_b0, float prior_b1, 
                    std::string transition_dist="beta_binom",
                    std::string test_statistic="wald",
                    float act_l=0.2, float act_u=0.8, int act_n=7);

	void solve();

	void to_sqlite(char* db_fname, int chunk_size);

	// Destructor
	~TrialMDP();
};

#endif
