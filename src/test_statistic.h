// test_statistic.h
// (c) 2020-10 David Merrell
//
// The TestStatistic struct defines an interface.
// It abstracts away the test statistic used by 
// the optimizer, making it easy for us to use different
// test statistics in the future.

#ifndef _DPM_TEST_STATISTIC_H
#define _DPM_TEST_STATISTIC_H

#include <cmath>
#include <limits>
#include <string>

/**
 * Abstract base class.
 * Just defines an interface and a factory method.
 */ 
class TestStatistic {
    public:

      TestStatistic(){ return; }

      static TestStatistic* make_test_statistic(std::string name);

      virtual float operator()(float p_a, float p_b, float P, float N_a, float N_b, float N) = 0;
};



/**
 * A Wald statistic for superiority test p_a > p_b
 * (against null p_a = p_b). 
 * Implements the TestStatistic interface.
 */
class WaldStatistic : public TestStatistic {
    public:

      WaldStatistic(){ return; }

      float operator()(float p_a, float p_b, float P, float N_a, float N_b, float N){

          // The Wald statistic
          if (N_a == 0.0 || N_b == 0.0){
              return -std::numeric_limits<float>::infinity();
          }
          float W = 0.0;
          if (P != 0.0 && P != 1.0){
              W = ( pow(p_a - p_b, 2.0) / (P * (1.0 - P)) ) * N_a*N_b / N;
          }
          // (is asymptotically chi-square(df=1) under null hypothesis)
      
          // For now, just return the Wald statistic.
          // We probably ought to return a different quantity
          // that allows us to talk more naturally about tradeoffs between
          // statistical power and other terms of the objective--
          // some monotone transformation of W.
          // E.g., the p-value?
          return W;   
      }

};


#endif

