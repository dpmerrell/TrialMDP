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
#include "block_rar_opt.h"

struct InputTable{
    int n_patients;
    int block_incr;

    float error_cost;
    float block_cost;

    char* sqlite_fname;
};

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

InputTable parse_args(int argc, char* argv[]){

    InputTable result;
    result.n_patients = std::stoi(argv[1]);
    result.block_incr = std::stoi(argv[2]);
    result.error_cost = std::stof(argv[3]);
    result.block_cost = std::stof(argv[4]);
    result.sqlite_fname = argv[5];
    
    return result;
}


int main(int argc, char* argv[]){

	// Parse command line args
	InputTable args = parse_args(argc, argv);

	std::cout << "about to initialize solver" << std::endl;
	BlockRAROpt solver = BlockRAROpt(args.n_patients, args.block_incr,
			                 args.error_cost, args.block_cost);
	std::cout << "Initialized solver; about to solve" << std::endl;

	solver.solve();
	std::cout << "solver completed" << std::endl;

	solver.to_sqlite(args.sqlite_fname, 10);
	std::cout << "Saved to file: " << args.sqlite_fname << std::endl;

	return 0;
}


