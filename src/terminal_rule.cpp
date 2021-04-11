

#include "terminal_rule.h"
#include <iostream>

////////////////////////
// Factory method
////////////////////////

TerminalRule* TerminalRule::make_terminal_rule(std::string test_stat, float failure_cost){
    if (test_stat == "wald"){
      return new WaldFailureTerminalRule(failure_cost);
    }
    else if (test_stat == "cmh" || test_stat == "harmonic_mean" || test_stat == "harmonic_mean_2"){
      return new RescaledFailureTerminalRule(failure_cost);
    }
    else{
      std::cerr << test_stat << " not a valid value for test statistic." << std::endl;
      throw(1);
    }
}
