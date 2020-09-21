// block_rar_opt.h
// (c) David Merrell 2020-08
//
// Class that represents a block RAR design
// problem instance.

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
	
        BlockRARTable* results_table;
        StateIterator* state_iterator;
	ActionIterator* action_iterator;

	// Methods
	StateResult terminal_reward(ContingencyTable ct);
	StateResult max_expected_reward(int cur_idx, ContingencyTable ct);

    public:

        // Constructor
	BlockRAROpt(int n_patients, int block_incr, float error_cost, float block_cost);

	void solve();

	void to_sqlite(char* db_fname, int chunk_size);

	// Destructor
	~BlockRAROpt();
};

#endif
