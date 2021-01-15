// transition_dist.h
// (c) 2021-01 David Merrell
//
// Defines a class representing transition distributions. 


#ifndef _TRANSITION_DIST_H
#define _TRANSITION_DIST_H

#include "contingency_table.h"
#include <vector>

class TransitionDist{

    public:
        // empty constructor
        TransitionDist(){ return; }
        
        // factory method
        static TransitionDist* make_transition_dist(std::string tr_dist_type,
                                                    float pr_a0, float pr_a1,
                                                    float pr_b0, float pr_b1);
        
        virtual float prob(int a, int b) = 0;
        
        virtual void set_state_action(ContingencyTable state, 
                                      short unsigned int size_a,
                                      short unsigned int size_b) = 0;
        

};



class BinomTransitionDist : public TransitionDist{

    private:
        float prior_a0;
        float prior_a1;
        float prior_b0;
        float prior_b1;

        std::vector<float> a_probs;
        std::vector<float> b_probs;

    public:
        BinomTransitionDist(float pr_a0, float pr_a1,
                            float pr_b0, float pr_b1); 
        void set_state_action(ContingencyTable state,
                              short unsigned int size_a,
                              short unsigned int size_b);
        
        float prob(int a, int b){
            return a_probs[a]*b_probs[b];
        }
        
};


class BetaBinomTransitionDist : public TransitionDist{

    private:
        float prior_a0;
        float prior_a1;
        float prior_b0;
        float prior_b1;
        
        std::vector<float> a_probs;
        std::vector<float> b_probs;

    public:
        BetaBinomTransitionDist(float pr_a0, float pr_a1,
                                float pr_b0, float pr_b1);

        void set_state_action(ContingencyTable state,
                              short unsigned int size_a,
                              short unsigned int size_b);
        
        float prob(int a, int b){
            return a_probs[a]*b_probs[b];
        }
};

#endif
