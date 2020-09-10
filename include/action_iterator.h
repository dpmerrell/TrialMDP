
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
        unsigned int size_idx;

	int block_size;
	int act_a;
	int act_b;


    public:
        ActionIterator(float min_ratio, float max_ratio, int n_ratios,
                       std::vector<int> n_vec,
                       int n_idx);

	ActionIterator(){
            alloc_idx = 0;
	    cur_size_idx = 0;
	    size_idx = 0;
	    block_size = 0;
	    act_a = 0;
	    act_b = 0;
	}

        void reset(int n_idx);
        
        bool not_finished();
        void advance();

        int get_block_size();
        int action_a();
        int action_b();

	int get_cur_size_idx();
};

#endif
