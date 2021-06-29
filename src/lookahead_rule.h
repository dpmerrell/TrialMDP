// lookahead_rule.h
// (c) 2021-02 David Merrell
//
// The dynamic programming algorithm populates
// a big table (TrialMDPTable). Each entry (StateResult)
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
#include "contingency_table.h"
#include <vector>
#include <iostream>
#include <limits>

/**
*  Abstract base class
**/
class LookaheadRule{

    public:
        virtual void operator()(std::vector<float>& current_values,
                                ContingencyTable& current_state,
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

        void operator()(std::vector<float>& current_values,
                        ContingencyTable& current_state,
                        int action_a, int action_b,
                        int n_a, int n_b,
                        StateResult& next,
                        int idx){
            current_values[idx] = next.values[idx];
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

        void operator()(std::vector<float>& current_values,
                        ContingencyTable& current_state,
                        int action_a, int action_b,
                        int n_a, int n_b,
                        StateResult& next,
                        int idx){
            current_values[idx] = next.values[idx] + a;
        }

};


/*
* This lookahead computes a rescaled estimate of the 
* CMH statistic's expected value:
*     V = (sum w_i) / pq / N
* where each w_i is the harmonic mean of N_{A,i} and N_{B,i}. 
*/
class ScaledCMH : public LookaheadRule{

    private:

        int v_idx;
        //int numerator_idx;

        float v;
        //float numerator;
        float N_inv;

        void compute_v(ContingencyTable& current_state,
                       int action_a, int action_b,
                       int n_a, int n_b,
                       StateResult& next){


            if (action_a == 0 || action_b == 0){
                v = -std::numeric_limits<float>::infinity();
                //numerator = 0.0;
                return;
            }

            float T = action_a + action_b;
            float w = action_a*action_b / T;

            float p_a = float(current_state.a1 + n_a + 1) / float(current_state.a1 + current_state.a0 + action_a + 2.0);
            float q_a = 1.0 - p_a; 
            float p_b = float(current_state.b1 + n_b + 1) / float(current_state.b1 + current_state.b0 + action_b + 2.0);
            float q_b = 1.0 - p_b;
            float pq_hat = 0.25*(p_a + p_b)*(q_a + q_b);
 
            //float num_next = next.values[numerator_idx];
            float v_next = next.values[v_idx];
            //numerator = w + num_next;

            v = v_next + (N_inv * w / pq_hat);
            //v = N_inv * numerator / pq_hat;

        }


    public:

        //ScaledCMH(int v_i, int num_i, int n_pat){
        ScaledCMH(int v_i, int n_pat){
            v_idx = v_i;
            //numerator_idx = num_i;
            N_inv = 1.0 / float(n_pat);
        }


        void operator()(std::vector<float>& current_values,
                        ContingencyTable& current_state,
                        int action_a, int action_b,
                        int n_a, int n_b,
                        StateResult& next,
                        int idx){

            if(idx == v_idx){
                compute_v(current_state, 
                          action_a, action_b,
                          n_a, n_b, next);
                current_values[idx] = v;
            //}else if(idx == numerator_idx){
            //    current_values[idx] = numerator;
            }else{
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

        void operator()(std::vector<float>& current_values,
                        ContingencyTable& current_state,
                        int action_a, int action_b,
                        int n_a, int n_b,
                        StateResult& next,
                        int idx){
            current_values[idx] = current_values[a_idx]*a + current_values[b_idx]*b + current_values[c_idx]*c;
        }

        

};


#endif
