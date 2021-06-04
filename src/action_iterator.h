// action_iterator.h
// (c) 2020 David Merrell
//
// Defines an iterator over possible blocks.
// I.e., it iterates over the possible 
//   * block sizes and
//   * treatment allocations
// 
// given the number of patients we've already treated
// and the remaining number of patients in the trial.
//

#ifndef _ACTION_ITERATOR_H
#define _ACTION_ITERATOR_H

#include "contingency_table.h"
#include <vector>

class ActionIterator{

    private:
        // These variables encode the state of the iterator
        std::vector<int> size_vec;
        std::vector<float> ratio_vec;
	std::vector<int> alloc_vec;
        unsigned int alloc_idx;
        unsigned int cur_size_idx;
        unsigned int next_size_idx;

	int block_size;
        int min_size;
	int act_a;
	int act_b;


    public:
	// Constructors
        ActionIterator(float min_ratio, float max_ratio, int n_ratios,
                       std::vector<int> n_vec,
                       int min_s,
                       int n_idx);
	ActionIterator(){
            alloc_idx = 0;
	    cur_size_idx = 0;
	    next_size_idx = 0;
	    block_size = 0;
            min_size = 0;
	    act_a = 0;
	    act_b = 0;
	}
        ActionIterator(const ActionIterator& other){
            alloc_idx = other.alloc_idx;
	    cur_size_idx = other.cur_size_idx;
	    next_size_idx = other.next_size_idx;
	    block_size = other.block_size;
	    act_a = other.act_a;
	    act_b = other.act_b;
            size_vec = other.size_vec;
            min_size = other.min_size;
            ratio_vec = other.ratio_vec;
	    alloc_vec = other.alloc_vec;
	}

        void reset(int n_idx);
        
        bool not_finished();
        void advance();

        int get_block_size();
        int action_a();
        int action_b();

	int get_next_size_idx();
};

#endif
