
#ifndef _ACTION_ITERATOR_H
#define _ACTION_ITERATOR_H

#include "contingency_table.h"
#include <vector>

class ActionIterator{

    private:
        // These variables encode the state of the iterator
        std::vector<float> ratio_vec;
        int ratio_idx;
        std::vector<int> size_vec;
        int cur_n_idx;
        int act_n_idx;

	int act_n;
	int act_a;
	int act_b;

	// TODO: differing ratios may yield equal (integer) allocations.
	// We need to modify this to eliminate that redundancy.

    public:
        ActionIterator(float min_ratio, float max_ratio, int n_ratios,
                       std::vector<int> n_vec,
                       int n_idx);

        void reset(int n_idx);
        
        bool is_finished();
        void advance();

        int action_size();
        int action_a();
        int action_b();
};

#endif
