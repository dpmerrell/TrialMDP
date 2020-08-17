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
#include "block_rar_iterator.h"
#include <string>

class BlockRAROpt{

    private:
	// Data
        int n_patients;
        int block_incr;
        BlockRARTable results_table;
        BlockRARIterator state_iterator;
	
	// Methods
	void initialize();
	StateResult max_expected_reward(ContingencyTable ct);

    public:

        // Constructor
	BlockRAROpt(int n_patients, int block_incr);

	void solve();

	void save_results(std::string out_filename);

};

#endif
