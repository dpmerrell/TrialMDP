
#ifndef _STATE_RESULT_H
#define _STATE_RESULT_H

struct StateResult{
    // Optimal action
    int next_block_size;
    float next_allocation_ratio;

    // terms of the reward function
    float statistical_power;
    float n_failures;
    float remaining_blocks;

    // combined reward value
    float reward;
};


#endif
