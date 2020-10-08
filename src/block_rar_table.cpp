// block_rar_table.cpp
// (c) 2020 David Merrell
//

#include "block_rar_table.h"
#include "contingency_table.h"
#include "state_result.h"
#include <iostream>

std::vector<int> build_n_vec(unsigned int n_max, unsigned int n_incr){
    unsigned int len = 1 + (n_max / n_incr);
    std::vector<int> result (len);
    int k = 0;
    for(unsigned int i = 0; i <= n_max; i += n_incr){
        result[k] = i;
	k++;
    }
    // If n_incr doesn't divide n_max neatly, then
    // set the last entry to n_max -- its distance
    // from the previous entry will be somewhat
    // farther than n_incr.
    result[result.size()-1] = n_max;
    return result;
}


int size_fn(int n){
    return (n+1)*(n+2)*(n+3)/6 ;
}

BlockRARTable::BlockRARTable(int n_max, int n_incr){
    n_vec = build_n_vec(n_max, n_incr);
    results = std::vector< std::unordered_map<ContingencyTable, StateResult, CTHash>* >();
    
    for(unsigned int i = 0; i < n_vec.size(); i++){
        //int n_buckets = size_fn(n_vec[i]) / 5;
        results.push_back( new std::unordered_map<ContingencyTable, StateResult, CTHash> );//(n_buckets) ); 
    }

}


