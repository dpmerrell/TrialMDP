

#include "terminal_reward.h"
#include <iostream>

////////////////////////
// Factory method
////////////////////////

TerminalReward* TerminalReward::make_terminal_reward(std::string name, float failure_cost){
    if (name == "wald_failure"){
      return new WaldFailureTerminalReward(failure_cost);
    }else{
      std::cerr << name << " not a valid value for terminal reward." << std::endl;
      throw(1);
    }
}
