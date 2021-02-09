

#include "terminal_rule.h"
#include <iostream>

////////////////////////
// Factory method
////////////////////////

TerminalRule* TerminalRule::make_terminal_rule(std::string name, float failure_cost){
    if (name == "wald_failure"){
      return new WaldFailureTerminalRule(failure_cost);
    }else{
      std::cerr << name << " not a valid value for terminal reward." << std::endl;
      throw(1);
    }
}
