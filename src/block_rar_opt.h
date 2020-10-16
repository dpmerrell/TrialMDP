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
//   * error_cost: the cost of assigning a patient to the inferior treatment
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
#include <string>

class BlockRAROpt{

    private:
	// Data
        int n_patients;
        int block_incr;
	float error_cost;
	float block_cost;
        float smoothing;
	
        BlockRARTable* results_table;
        StateIterator* state_iterator;
	ActionIterator* action_iterator;

	// Methods
	StateResult terminal_reward(ContingencyTable ct);
	StateResult max_expected_reward(int cur_idx, ContingencyTable ct);

    public:

        // Constructor
	BlockRAROpt(int n_patients, int block_incr, float error_cost, float block_cost, float sm=1e-6);

	void solve();

	void to_sqlite(char* db_fname, int chunk_size);

	// Destructor
	~BlockRAROpt();
};

#endif
