// state_iterator.h
// (c) 2020-08 David Merrell
//
// Defines the StateIterator class -- an iterator
// over all possible contingency tables of size 1 through
// N_patients (though it traverses them in order of 
// *decreasing* size).
// 

# ifndef _STATE_ITERATOR_H
# define _STATE_ITERATOR_H

#include <vector>
#include "contingency_iterator.h"
#include "contingency_table.h"
#include "block_rar_table.h"

class StateIterator{

    private:
        std::vector<int> n_vec;
        int cur_idx;
        ContingencyIterator* cur_iter;
	int cur_count;

    public:
	// constructors 
	StateIterator(BlockRARTable tab);
	StateIterator();
	StateIterator(const StateIterator& si2);

        ContingencyTable value();
	int get_cur_idx();
        void advance();
        bool not_finished();
        ~StateIterator();	

};

# endif
