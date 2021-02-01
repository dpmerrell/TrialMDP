
#include "transition_reward.h"
#include <iostream>
#include <string>

//////////////////////
// FACTORY METHOD
//////////////////////


TransitionReward* TransitionReward::make_transition_reward(std::string name, float block_cost, int n_patients){

    if (name == "block_cost"){
      return new BlockCostReward(block_cost);
    }else{
      std::cerr << name << " not a valid value for transition reward." << std::endl;
      throw(1);
    }
}

