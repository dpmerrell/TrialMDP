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
#include "state_iterator.h"

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


	// Reference cases:
	// For arguments (patients=500, increment=25) 
	// the results table takes up ~9GB of RAM.
	// That gives a sense of the algorithm's memory cost.
	// Similarly: (200, 1) -> 5GB of RAM.
	// A reasonably good workstation can handle these loads.
	//
	// The time cost of the algorithm is a whole other question.
	// It will depend on whether we figure out any clever tricks:
	// pruning, etc. Parallelism will help.
	std::vector<int> arg_vec = parse_args(argc, argv);

	//std::cout << "about to initialize table" << std::endl;
	BlockRARTable result_table = BlockRARTable(arg_vec[0], arg_vec[1]);
	std::cout << "Initialized table" << std::endl;

	StateIterator s_it = StateIterator(result_table);
	std::cout << "Initialized state iterator" << std::endl;

	std::cout << v_to_str(result_table.get_n_vec()) << std::endl;

	int iter = 0;
	while(s_it.not_finished()){
	    std::cout << iter << std::endl;
	    int cur_idx = s_it.get_cur_idx();
	    std::cout << "SUCCESSFULLY GOT IDX: " << cur_idx << std::endl;

	    ContingencyTable ctab = s_it.value();
	    ctab.pretty_print();
            result_table(cur_idx, ctab) = StateResult {0, 0.0, 0.0, 0.0, 0.0, 0.0};
	    std::cout << "SUCCESSFULLY ADDED STATE TO TABLE" << std::endl;
            s_it.advance();
	    iter++;
	    bool is_fin = !s_it.not_finished();
	    std::cout << "SUCCESSFULLY TESTED NOT_FINISHED: " << !is_fin << std::endl;
	}
	std::cout << "SUCCESSFULLY BROKE LOOP" << std::endl;
	return 0;
}


