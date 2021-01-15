// transition_iterator.h
// (c) 2020-09 David Merrell 
// 
// Defines an iterator over the possible transitions
// that may occur, given the design of the next block.
//
// The iterator yields every possible outcome, as well
// as their probabilities. This allows the algorithm to 
// compute expected values of outcomes and maximize 
// expected reward.

#ifndef _TRANSITION_ITERATOR_H
#define _TRANSITION_ITERATOR_H

#include "contingency_table.h"
#include "transition_dist.h" 
#include <vector>
/**
 * For a given state and action, iterate the 
 * possible outcomes with their corresponding probabilities
 * and rewards.
 */
class TransitionIterator {

    private:
        ContingencyTable cur_table;
	short unsigned int a_size;
	short unsigned int b_size;
	short unsigned int a_counter;
	short unsigned int b_counter;
        TransitionDist* transition_distribution;

    public:
        TransitionIterator(ContingencyTable ct, int a_A, int a_B);
	float prob();
	ContingencyTable value();
	bool not_finished();
	void advance();

        short unsigned int get_a_counter(){ return a_counter; }
        short unsigned int get_b_counter(){ return b_counter; }

};


#endif
