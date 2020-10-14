// block_rar_table.h
// (c) David Merrell 2020-08
//
// A class that stores all of the results
// for a block RAR RCT design problem. 
// I.e., for every possible state it stores
// the best action and the expected reward.
// 
// It's essentially a vector of hash maps;
// one map for every possible size of contingency table. 

#ifndef _BLOCK_RAR_TABLE_H
#define _BLOCK_RAR_TABLE_H

#include <unordered_map>
#include <vector>
#include "contingency_table.h"
#include "state_result.h"
#include <iostream>

class BlockRARTable{

    private:
        std::vector< std::unordered_map<ContingencyTable, StateResult, CTHash>* > results;
	std::vector<int> n_vec;

    public:
        BlockRARTable(int n_max, int n_incr);

	BlockRARTable(){
            results = std::vector< std::unordered_map<ContingencyTable, StateResult, CTHash>* >();
	    n_vec = std::vector<int>();
	}

	BlockRARTable(const BlockRARTable& other){
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


