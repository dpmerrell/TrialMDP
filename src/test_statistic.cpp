

#include "test_statistic.h"
#include <iostream>

////////////////////////
// Factory method
////////////////////////

TestStatistic* TestStatistic::make_test_statistic(std::string name){
    if (name == "wald"){
      return new WaldStatistic();
    }else{
      std::cerr << name << " not a valid value for test statistic." << std::endl;
      throw(1);
    }
}
