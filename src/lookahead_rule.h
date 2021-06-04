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


        void operator()(std::vector<float>& current_values,
                        ContingencyTable& current_state,
                        int action_a, int action_b,
                        int n_a, int n_b,
                        StateResult& next,
                        int idx){

            if(idx == cmh_idx){
                compute_cmh(action_a, action_b,
                            n_a, n_b, next);
                current_values[idx] = cmh;
            }
            else if(idx == numerator_idx){
                current_values[idx] = numerator;
            }
            else if(idx == denom_idx){
                current_values[idx] = denom;
            }
            else{
                throw 1;
            }
        }


};


/*
* This lookahead computes the Cochran-Mantel-Haenszel
* statistic for the current and subsequent blocks.
*/
class ScaledCMHStatisticLR : public LookaheadRule{

    private:

        int cmh_idx;
        int numerator_idx;
        int denom_idx;

        float N;
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
                cmh = numerator*numerator / denom / N;
            }

        }


    public:

        ScaledCMHStatisticLR(int cmh_i, int num_i, int den_i, int N_in){
            cmh_idx = cmh_i;
            numerator_idx = num_i;
            denom_idx = den_i;
            N = float(N_in);
        }


        void operator()(std::vector<float>& current_values,
                        ContingencyTable& current_state,
                        int action_a, int action_b,
                        int n_a, int n_b,
                        StateResult& next,
                        int idx){

            if(idx == cmh_idx){
                compute_cmh(action_a, action_b,
                            n_a, n_b, next);
                current_values[idx] = cmh;
            }
            else if(idx == numerator_idx){
                current_values[idx] = numerator;
            }
            else if(idx == denom_idx){
                current_values[idx] = denom;
            }
            else{
                throw 1;
            }
        }

};


/*
* This lookahead computes the Cochran-Mantel-Haenszel
* statistic for the current and subsequent blocks.
* It uses a second-order approximation to compute the 
* expected value
*/
class ScaledCMH2ndOrderLR : public LookaheadRule{

    private:

        int cmh_idx;
        int numerator_sqrt_idx;
        int numerator_idx;
        int denom_idx;

        float N;
        float cmh;
        float numerator_sqrt;
        float numerator;
        float denom;

        void compute_cmh(int action_a, int action_b,
                         int n_a, int n_b,
                         StateResult& next){


            if (action_a == 0 || action_b == 0){
                cmh = -std::numeric_limits<float>::infinity();
                numerator_sqrt = 0.0;
                numerator = 0.0;
                denom = 0.0;
                return;
            }

            float T = action_a + action_b;
            float num_sqrt_next = next.values[numerator_sqrt_idx];
            float num_next = next.values[numerator_idx];
            float n = n_a + n_b;
            float nterm = (n_a - float(action_a)*n/T);
            float dterm = float(action_a*action_b*n*(T-n))/(T*T*(T-1));
 
            numerator_sqrt = nterm + num_sqrt_next;
            //numerator = numerator_sqrt*numerator_sqrt;
            numerator = nterm * ( nterm + 2.0*num_sqrt_next ) + num_next;

            float denom_next = next.values[denom_idx];
            denom = dterm + denom_next;

            cmh = 0.0;
            if(denom != 0.0){
                cmh = numerator / denom / N;
            }

        }


    public:

        ScaledCMH2ndOrderLR(int cmh_i, int num_sq_i, int num_i, int den_i, int N_in){
            cmh_idx = cmh_i;
            numerator_sqrt_idx = num_sq_i;
            numerator_idx = num_i;
            denom_idx = den_i;
            N = float(N_in);
        }


        void operator()(std::vector<float>& current_values,
                        ContingencyTable& current_state,
                        int action_a, int action_b,
                        int n_a, int n_b,
                        StateResult& next,
                        int idx){

            if(idx == cmh_idx){
                compute_cmh(action_a, action_b,
                            n_a, n_b, next);
                current_values[idx] = cmh;
            }
            else if(idx == numerator_sqrt_idx){
                current_values[idx] = numerator_sqrt;
            }
            else if(idx == numerator_idx){
                current_values[idx] = numerator;
            }
            else if(idx == denom_idx){
                current_values[idx] = denom;
            }
            else{
                throw 1;
            }
        }

};


/*
*  This lookahead rule computes a harmonic mean
*  of N_A, N_B over *all* blocks.
*/
class HarmonicMeanLR : public LookaheadRule{

    private:

        int hm_idx;
        int inv_idx;

        float hm;
        float inv;
        float N;

        void compute_hm(int action_a, int action_b,
                        int n_a, int n_b,
                        StateResult& next){

            if (action_a == 0 || action_b == 0){
                hm = -std::numeric_limits<float>::infinity();
                inv = 0.0;
                return;
            }

            inv = next.values[inv_idx] + (1.0/action_a) + (1.0/action_b);
            hm = 1.0 / inv;
        }


    public:

        HarmonicMeanLR(int hm_i, int inv_i){
            hm_idx = hm_i;
            inv_idx = inv_i;
        }


        void operator()(std::vector<float>& current_values,
                        ContingencyTable& current_state,
                        int action_a, int action_b,
                        int n_a, int n_b,
                        StateResult& next,
                        int idx){

            if(idx == hm_idx){
                compute_hm(action_a, action_b,
                           n_a, n_b, next);
                current_values[idx] = hm;
            }
            else if(idx == inv_idx){
                current_values[idx] = inv;
            }
            else{
                throw 1;
            }
        }

};


/*
*  This lookahead computes the harmonic mean of 
*  N_A and N_B for this block, and subsequent blocks
*/
class BlockHarmonicMeanLR : public LookaheadRule{

    public:

        BlockHarmonicMeanLR(){ return; } 


        void operator()(std::vector<float>& current_values,
                        ContingencyTable& current_state,
                        int action_a, int action_b,
                        int n_a, int n_b,
                        StateResult& next,
                        int idx){
            current_values[idx] = float(action_a * action_b)/float(action_a + action_b) + next.values[idx];
        }


};


/*
* This lookahead computes a function of the harmonic mean:
*     V = (sum w_i)^2 / sum (w_i^2 * (pq)
* where each w_i is the harmonic mean of N_{A,i} and N_{B,i}. 
*/
class HarmonicMeanDSQ : public LookaheadRule{

    private:

        int v_idx;
        int numerator_sqrt_idx;
        int numerator_idx;
        int denom_idx;

        float v;
        float numerator_sqrt;
        float numerator;
        float denom;
        float N_inv;

        void compute_v(ContingencyTable& current_state,
                       int action_a, int action_b,
                       int n_a, int n_b,
                       StateResult& next){


            if (action_a == 0 || action_b == 0){
                v = -std::numeric_limits<float>::infinity();
                numerator_sqrt = 0.0;
                numerator = 0.0;
                denom = 0.0;
                return;
            }

            float T = action_a + action_b;
            float w = action_a*action_b / T;

            float p_a = float(current_state.a1 + 1) / float(current_state.a1 + current_state.a0 + 2.0);
            float q_a = 1.0 - p_a; 
            float p_b = float(current_state.b1 + 1) / float(current_state.b1 + current_state.b0 + 2.0);
            float q_b = 1.0 - p_b;
            float pq_hat = (p_a*q_a/action_a) + (p_b*q_b/action_b);
            
            float num_sqrt_next = next.values[numerator_sqrt_idx];
            float num_next = next.values[numerator_idx];
 
            numerator_sqrt = w + num_sqrt_next;
            numerator = w * ( w + 2.0*num_sqrt_next ) + num_next;

            float denom_next = next.values[denom_idx];
            denom = (w*w*pq_hat) + denom_next;

            v = 0.0;
            if(denom != 0.0){
                v = N_inv * numerator / denom;
            }

        }


    public:

        HarmonicMeanDSQ(int v_i, int num_sq_i, int num_i, int den_i, int n_pat){
            v_idx = v_i;
            numerator_sqrt_idx = num_sq_i;
            numerator_idx = num_i;
            denom_idx = den_i;
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
            }
            else if(idx == numerator_sqrt_idx){
                current_values[idx] = numerator_sqrt;
            }
            else if(idx == numerator_idx){
                current_values[idx] = numerator;
            }
            else if(idx == denom_idx){
                current_values[idx] = denom;
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
