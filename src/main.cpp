// main.cpp
// (c) 2020-08 David Merrell
//
// Dynamic programming algorithm for 
// block RAR RCT design.
//

#include <iostream>
#include "contingency_table.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <iterator>

std::string v_to_str(std::vector<float>& v){
    std::string result = "[";
    
    for (std::vector<float>::iterator it = v.begin(); it != v.end(); it++){
       result += std::to_string(*it);
       if (std::next(it) != v.end()){
           result += ", ";       
       }
    }
    result += "]";

    return result;
}	

int main(int argc, char* argv[]){

	std::cout << "Hello, world!" << std::endl;

	ContingencyTable my_table = {1,2,3,4};

	ContingencyTable other_table = {1,2,3,4};

        ContingencyTable different_table = {1,2,3,0};

	std::cout << "SHOULD BE TRUE:\t" << (my_table == other_table) << std::endl;
	std::cout << "SHOULD BE FALSE:\t" << (my_table == different_table) << std::endl;

	my_table.pretty_print();

	std::unordered_map<ContingencyTable, std::vector<float>, CTHash> my_map;

	my_map[my_table] = std::vector<float>(5, 3.14);
	my_map[different_table] = std::vector<float>(5, 2.7);
	
	std::cout << "SHOULD BE A VECTOR:\t" << v_to_str(my_map[my_table]) << std::endl;
	std::cout << "SHOULD BE A DIFFERENT VECTOR:\t" << v_to_str(my_map[different_table]) << std::endl;

        // std::cout << "SHOULD BE AN UNORDERED_MAP:\t" << my_map << std::endl;
}


