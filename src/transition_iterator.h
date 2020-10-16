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
	std::vector<float> a_probs;
	std::vector<float> b_probs;

    public:
        TransitionIterator(ContingencyTable ct, int a_A, int a_B, float smoothing=1e-6);
	float prob();
	ContingencyTable value();
	bool not_finished();
	void advance();

};


#endif
