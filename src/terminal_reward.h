// terminal_reward.h
// (c) 2020-10 David Merrell
//
// The TerminalReward class defines an interface.
// It abstracts away the reward function evaluated by 
// the optimizer at terminal states, making it easy for 
// us to solve different problems in the future.

#ifndef _DPM_TERMINAL_REWARD_H
#define _DPM_TERMINAL_REWARD_H

#include "contingency_table.h"
#include "state_result.h"
#include <cmath>
#include <limits>
#include <string>

/**
 * Abstract base class.
 * Just defines an interface and a factory method.
 */ 
class TerminalReward {
    public:

      TerminalReward(){ return; }

      static TerminalReward* make_terminal_reward(std::string name, float f_cost);

      virtual StateResult operator()(ContingencyTable ct) = 0;
};



/**
 * This reward has two parts:
 * (1) A Wald statistic for superiority test p_a > p_b
 *     (against null p_a = p_b). 
 * (2) The number of excess failures
 * 
 * Implements the TerminalReward interface.
 */
class WaldFailureTerminalReward : public TerminalReward {

    private:
      float failure_cost;

    public:

      WaldFailureTerminalReward(float f_c){ 
        failure_cost = f_c;
        return; 
      }

      StateResult operator()(ContingencyTable ct){
    
          // some useful row sums:
          float N_a = ct.a0 + ct.a1;
          float N_b = ct.b0 + ct.b1;
          float N = N_a + N_b;
          float p_a = 0.5;
          if (N_a != 0.0){
              p_a = float(ct.a1) / N_a;
          } 
          float p_b = 0.5;
          if (N_b != 0.0){
              p_b = float(ct.b1) / N_b;
          }
          float P = 0.5;
          if (N != 0.0){
              P = float(ct.a1 + ct.b1) / N;
          }

          // The Wald statistic
          float W = 0.0;
          if (N_a == 0.0 || N_b == 0.0){
              W = -std::numeric_limits<float>::infinity();
          }
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

          // Compute the "failures": the expected number of patients
          // that would have received a positive outcome, had they 
          // been given the superior treatment.
          float err;
          if(p_a >= p_b){
              err = (p_a - p_b)*N_b;
          } else{
              err = (p_b - p_a)*N_a;
          }

          // Compute the linear combination of those
          // factors
          float rwd = W - failure_cost*err; // - block_cost*remaining_blocks;
                                              // ^^^This is zero for terminal states
          
          StateResult result = StateResult {0, 0, W, err, 0.0, rwd};

          return result;
      }



};


#endif

