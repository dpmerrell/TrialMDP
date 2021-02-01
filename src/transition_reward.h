// transition_reward.h
// (c) 2021-01 David Merrell
// 
// The TransitionReward class computes rewards
// accrued by the agent as it transitions from one state
// to another.

#ifndef __TRANSITION_REWARD_H
#define __TRANSITION_REWARD_H

#include <string>

/**
 * Abstract base class
 */ 
class TransitionReward{

    public:

        static TransitionReward* make_transition_reward(std::string name, float b_c, int n_p);

        virtual float operator()(int a_A, int a_B, int n_A, int n_B) = 0;

        TransitionReward(){ return; }

};



/**
 * Simple reward function that penalizes each 
 * transition by the same amount.
 */
class BlockCostReward : public TransitionReward{

    private:
        float rwd;

    public:

        BlockCostReward(float block_cost){
            rwd = -1.0*block_cost;
            return;
        }

        float operator()(int a_A, int a_B, int n_A, int n_B){
            return rwd;
        }

};


/////**
//// * StratifiedWaldReward
//// *
//// */
////class StratifiedWaldReward : public TransitionReward{
////
////    private:
////
////    public:
////
////        StratifiedWaldReward(float block_cost)
////
////};


#endif
