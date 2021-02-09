// lookahead_rule.h
// (c) 2021-02 David Merrell
//
// The dynamic programming algorithm populates
// a big table (BlockRARTable). Each entry (StateResult)
// of that table is a set of quantities (e.g., TotalReward, RemainingBlocks).
// Each of those quantities q_i is the expected value of some
// function of the next state:
// 
// q_i = E[ f(s') ]
//
// A LookaheadRule defines that function, f.


#ifndef __LOOKAHEAD_RULE_H_
#define __LOOKAHEAD_RULE_H_

#include "state_result.h"

/**
*  Abstract base class
**/
class LookaheadRule{

    public:
        virtual float operator()(StateResult& next) = 0;

};


/**
*  Simply return the value from the next state 
**/
class IdentityLR : public LookaheadRule{

    private:
        int idx;

    public:

        IdentityLR(){
            idx = 0;
        }

        IdentityLR(int index){
            idx = index;
        }

        float operator()(StateResult& next){
            return next.values[idx];
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
        int idx;
        float a;

    public:
        
        AddConstLR(){
            idx = 0;
            a = 0.0;
        }

        AddConstLR(int index, float addend){
            idx = index;
            a = addend;
        }

        float operator()(StateResult& next){
            return next.values[idx] + a;
        }

};

#endif
