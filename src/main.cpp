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

std::vector<int> parse_args(int argc, char* argv[]){

    std::vector<int> result;
    for(int i=1; i < argc; i++){
        result.push_back(std::stoi(argv[i]));
    }
    return result;
}


int main(int argc, char* argv[]){


	// Reference case:
	// For arguments (patients=500, increment=25) 
	// the results table takes up ~9GB of RAM.
	// That gives a sense of the algorithm's memory cost.
	
	std::vector<int> arg_vec = parse_args(argc, argv);

	BlockRARTable result_table = BlockRARTable(arg_vec[0], arg_vec[1]);
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


