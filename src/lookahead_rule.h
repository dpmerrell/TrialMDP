// lookahead_rule.h
// (c) 2021-02 David Merrell
//
// The dynamic programming algorithm populates
// a big table (BlockRARTable). Each entry (StateResult)
// of that table is a set of quantities (e.g., TotalReward, RemainingBlocks).
// Each of those quantities q_i is the expected value of some
// function of the current state, action, and next state:
// 
// q_i = E[ f(s, a, s') ]
//
// A LookaheadRule defines that function, f.


#ifndef __LOOKAHEAD_RULE_H_
#define __LOOKAHEAD_RULE_H_

#include "state_result.h"
#include <vector>
#include <iostream>
#include <limits>

/**
*  Abstract base class
**/
class LookaheadRule{

    public:
        virtual void operator()(std::vector<float>& current,
                                 int action_a, int action_b,
                                 int n_a, int n_b,
                                 StateResult& next,
                                 int idx) = 0;

};


/**
*  Simply return the value from the next state 
**/
class IdentityLR : public LookaheadRule{

    public:

        void operator()(std::vector<float>& current,
                         int action_a, int action_b,
                         int n_a, int n_b,
                         StateResult& next,
                         int idx){
            current[idx] = next.values[idx];
        }

};


/*
* This one simply adds a constant to the value
* from the next state (for example, the reward for the
* current state is computed by looking at the reward
* for the next state, and subtracting a block cost.)
*/
class AddConstLR : public LookaheadRule{

    private:
        float a;

    public:
        
        AddConstLR(){
            a = 0.0;
        }

        AddConstLR(float addend){
            a = addend;
        }

        void operator()(std::vector<float>& current,
                         int action_a, int action_b,
                         int n_a, int n_b,
                         StateResult& next,
                         int idx){
            current[idx] = next.values[idx] + a;
        }

};


/*
* This lookahead computes the Cochran-Mantel-Haenszel
* statistic for the current and subsequent blocks.
*/
class CMHStatisticLR : public LookaheadRule{

    private:

        int cmh_idx;
        int numerator_idx;
        int denom_idx;

        float cmh;
        float numerator;
        float denom;

        void compute_cmh(int action_a, int action_b,
                         int n_a, int n_b,
                         StateResult& next){


            if (action_a == 0 || action_b == 0){
                cmh = -std::numeric_limits<float>::infinity();
                numerator = 0.0;
                denom = 0.0;
                return;
            }

            float T = action_a + action_b;
            float num_next = next.values[numerator_idx];
            float n = n_a + n_b;
            numerator = (n_a - float(action_a)*n/T) + num_next;

            float denom_next = next.values[denom_idx];
            denom = float(action_a*action_b*n*(T-n))/(T*T*(T-1)) + denom_next;

            cmh = 0.0;
            if(denom != 0.0){
                cmh = numerator*numerator/denom;
            }

        }


    public:

        CMHStatisticLR(int cmh_i, int num_i, int den_i){
            cmh_idx = cmh_i;
            numerator_idx = num_i;
            denom_idx = den_i;
        }


        void operator()(std::vector<float>& current,
                         int action_a, int action_b,
                         int n_a, int n_b,
                         StateResult& next,
                         int idx){

            if(idx == cmh_idx){
                compute_cmh(action_a, action_b,
                            n_a, n_b, next);
                current[idx] = cmh;
            }
            else if(idx == numerator_idx){
                current[idx] = numerator;
            }
            else if(idx == denom_idx){
                current[idx] = denom;
            }
            else{
                throw 1;
            }
        }


};


class LinCombLR : public LookaheadRule {

    private:
        int a_idx;
        int b_idx;
        int c_idx;
        float a;
        float b;
        float c;

    public:
        
        LinCombLR(){
            a_idx = 0;
            b_idx = 0;
            c_idx = 0;

            a = 0.0;
            b = 0.0;
            c = 0.0;
        }

        LinCombLR(int a_i, int b_i, int c_i,
                   float aa, float bb, float cc){
            a_idx = a_i;
            b_idx = b_i;
            c_idx = c_i;

            a = aa;
            b = bb;
            c = cc;
        }

        void operator()(std::vector<float>& current,
                         int action_a, int action_b,
                         int n_a, int n_b,
                         StateResult& next,
                         int idx){
            current[idx] = current[a_idx]*a + current[b_idx]*b + current[c_idx]*c;
        }

        

};


#endif
