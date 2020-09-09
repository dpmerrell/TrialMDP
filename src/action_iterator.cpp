
#include <vector>
#include <cmath>
#include "action_iterator.h"


ActionIterator::ActionIterator(float min_ratio, float max_ratio, int n_ratios,
                               std::vector<int> n_vec,
	                       int n_idx){
    
    // Populate the vector of allocation ratios
    ratio_vec = std::vector<float>(n_ratios, 0.0);
    float ratio_incr = (max_ratio - min_ratio)/(n_ratios - 1.0);
    for(unsigned int i=0; i < n_ratios; i++){
        ratio_vec[i] = min_ratio + (i*ratio_incr);
    }

    // Initiate index for allocation ratios
    ratio_idx = 0;
    
    // Initiate index for block sizes
    size_vec = n_vec;
    cur_n_idx = n_idx;
    act_n_idx = cur_n_idx + 1;
    act_n = size_vec[act_n_idx] - size_vec[cur_n_idx];
    act_a = int(round(min_ratio*act_n));
    act_b = act_n - act_a; 
}


void ActionIterator::reset(int n_idx){

    ratio_idx = 0;
    cur_n_idx = n_idx;
    act_n_idx = n_idx + 1;
    act_n = size_vec[act_n_idx] - size_vec[cur_n_idx];
    act_a = int(round(ratio_vec[0]*act_n));
    act_b = act_n - act_a;
}


bool ActionIterator::not_finished(){
    return ((ratio_idx < ratio_vec.size() - 1) || (act_n_idx < size_vec.size() - 1));
}


void ActionIterator::advance(){
    if(ratio_idx < ratio_vec.size() - 1){
        ratio_idx++;
	act_a = int(round(ratio_vec[ratio_idx]*act_n));
	act_b = act_n - act_a;
    } else{
        ratio_idx = 0;
	act_n_idx++;
	act_n = size_vec[act_n_idx] - size_vec[cur_n_idx];
	act_a = int(round(ratio_vec[0]*act_n));
        act_b = act_n - act_a;	
    }
}


int ActionIterator::action_size(){
    return act_n;
}


int ActionIterator::action_a(){
    return act_a;
}


int ActionIterator::action_b(){
    return act_b;
}



