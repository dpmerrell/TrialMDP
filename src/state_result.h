// state_result.h
// (c) David Merrell 2020-08
//
// A simple struct that stores the optimal
// policy for a contingency table. We use this
// to store the outputs of the algorithm.
// Each of these eventually becomes a row in a
// SQLite database.

#ifndef _STATE_RESULT_H
#define _STATE_RESULT_H

#include <iostream>

struct StateResult{
    // Optimal action
    int block_size;
    int a_allocation;

    // store all of the rewards/etc. that
    // the algorithm needs
    int n_values;
    float* values; 

    StateResult(){
        block_size = 0;
        a_allocation = 0;
        n_values = 0;
        values = NULL;
    }

    StateResult(int n_attributes){
        block_size = 0;
        a_allocation = 0;
        n_values = n_attributes;
        values = new float [n_attributes];
    }   

    StateResult& operator=(const StateResult& other){
        block_size = other.block_size;
        a_allocation = other.a_allocation;
        n_values = other.n_values;
        values = new float [n_values];
        for(unsigned int i = 0; i < n_values; ++i){
            values[i] = other.values[i];
        }
        return *this;
    }    

    StateResult(const StateResult& old){
        block_size = old.block_size;
        a_allocation = old.a_allocation;
        n_values = old.n_values;
        values = new float [n_values];
        for(unsigned int i = 0; i < n_values; ++i){
            values[i] = old.values[i];
        }
    }


    ~StateResult(){
        delete values;
    }
 
};


#endif
