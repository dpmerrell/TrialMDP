// state_result.h
// (c) David Merrell 2020-08
//

#ifndef _STATE_RESULT_H
#define _STATE_RESULT_H

#include <iostream>

struct StateResult{
    // Optimal action
    int next_block_size;
    int next_a_allocation;

    // terms of the reward function
    float statistical_power;
    float n_failures;
    float remaining_blocks;

    // combined reward value
    float reward;

    void pretty_print(){
        std::cout << next_block_size << ", "
		  << next_a_allocation << ", "
		  << statistical_power << ", "
		  << n_failures << ", "
		  << remaining_blocks << ", "
		  << reward << std::endl;
    }
};


#endif
