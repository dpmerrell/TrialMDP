// trial_mdp_table.h
// (c) David Merrell 2020-08
//
// A class that stores all of the results
// for an adaptive multistage trial design problem. 
// I.e., for every possible state it stores
// the best action and the expected reward.
// 
// It's essentially a vector of hash maps;
// one map for every possible size of contingency table. 

#ifndef _TRIAL_MDP_TABLE_H
#define _TRIAL_MDP_TABLE_H

#include <unordered_map>
#include <vector>
#include "contingency_table.h"
#include "state_result.h"
#include <iostream>

class TrialMDPTable{

    private:
        std::vector< std::unordered_map<ContingencyTable, StateResult, CTHash>* > results;
	std::vector<int> n_vec;

    public:
        TrialMDPTable(int n_max, int min_size, int n_incr);

	TrialMDPTable(){
            results = std::vector< std::unordered_map<ContingencyTable, StateResult, CTHash>* >();
	    n_vec = std::vector<int>();
	}

	TrialMDPTable(const TrialMDPTable& other){
            results = other.results;
	    n_vec = other.n_vec;
	}

	std::vector<int> & get_n_vec(){ return n_vec; }
        
	// Set one of the hash maps
	StateResult& operator ()(int idx, ContingencyTable ct) {return (*(results[idx]))[ct]; }
	
	// Get one of the hash maps
	StateResult& operator ()(int idx, ContingencyTable ct) const { return (*(results[idx]))[ct];}

};

#endif


