// main.cpp
// (c) 2020-08 David Merrell
//
// Dynamic programming algorithm for 
// block RAR RCT design.
//

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <iterator>
#include "block_rar_table.h"
#include "state_result.h"
#include "contingency_iterator.h"
#include "contingency_table.h"


std::string v_to_str(std::vector<int>& v){
    std::string result = "[";
    
    for (std::vector<int>::iterator it = v.begin(); it != v.end(); it++){
       result += std::to_string(*it);
       if (std::next(it) != v.end()){
           result += ", ";       
       }
    }
    result += "]";

    return result;
}	


int main(int argc, char* argv[]){

	// 500 patients; each block is a multiple of 25 patients in size.
	// Note: under these settings, the table takes up ~9GB of RAM.
	// That gives a sense of the memory cost of the algorithm.
	BlockRARTable result_table = BlockRARTable(500,25);
	std::cout << "Initialized table" << std::endl;

        std::vector<int> n_vals = result_table.get_n_vec();

	for (unsigned int j = 0; j < n_vals.size(); j++){
            std::cout << n_vals[j] << " PATIENTS:" << std::endl;

	    ContingencyIterator it = ContingencyIterator(n_vals[j]);

	    int k = 0;
	    while(true){

		result_table(j, it.value()) = StateResult {0, 0.0, 0.0, 0.0, 0.0, 0.0};
	        k++; 

                if(it.not_finished()){
	            it.advance();
	        }else{
	    	    break;
	        }	   

	    }
	    std::cout << "\t" << k << " TABLES" << std::endl;

	}
}


