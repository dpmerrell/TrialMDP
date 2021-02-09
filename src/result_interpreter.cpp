
#include "result_interpreter.h"
#include "contingency_table.h"
#include "state_result.h"
#include <cmath>
#include <string>
#include <vector>


ResultInterpreter::ResultInterpreter(std::string terminal_rule,
                                     std::string transition_reward){
    
    attr_names = std::vector<std::string>();

    // We'll store these values regardless of the
    // problem formulation.
    attr_names.push_back("TotalReward");

    attr_names.push_back("ExcessFailures");

    if(terminal_rule == "wald_failure"){
        attr_names.push_back("WaldStatistic");
    }
    if(transition_reward == "block_cost"){
        attr_names.push_back("RemainingBlocks");
    }

    n_attr = attr_names.size();

    attr_to_idx = make_dict(attr_names);


    return;
}


std::unordered_map< std::string, int > ResultInterpreter::make_dict(std::vector< std::string > names){
    std::unordered_map<std::string, int> result;
    for(unsigned int i=0; i < n_attr; ++i){
        result[names[i]] = i;
    }
    return result;
}


float ResultInterpreter::get_attr(StateResult& result, std::string attr_name){
    int attr_idx = attr_to_idx.at(attr_name);
    return result.values[attr_idx];
}


void ResultInterpreter::set_attr(StateResult& result, std::string attr_name, float new_value){
    result.values[attr_to_idx.at(attr_name)] = new_value;
}


std::string fl_to_str(float x){

    std::string s;
    if (std::isfinite(x)){
        s = std::to_string(x);
    }else{
        s = "NULL";
    }
    return s;
}


std::string ResultInterpreter::pretty_print_result(StateResult& res){

    //TODO propagate refactor here in this printout
    std::string print_str = "Action:\n"; 
    print_str += "\tBlock size: " + std::to_string(res.block_size) + "\n";
    print_str += "\tN_A: "+std::to_string(res.a_allocation)+"\n";
    print_str += "\tN_B: "+std::to_string(res.block_size - res.a_allocation)+"\n";

    print_str += "Expected values:\n";
    for(unsigned int i=0; i < n_attr; ++i){
        print_str += "\t" + attr_names[i] + ": " + fl_to_str(res.values[i]) + "\n";
    }

    return print_str;
}


std::string ResultInterpreter::sql_create_table(){

    std::string query = "CREATE TABLE RESULTS("\
    "A0 INT, A1 INT, B0 INT, B1 INT, "\
    "BlockSize INT, AAllocation INT, ";

    for(unsigned int i=0; i < n_attr; ++i){ 
        query += attr_names[i] + " REAL, "; 
    }
    query += "PRIMARY KEY (A0, A1, B0, B1));";

    return query;    
}


std::string ResultInterpreter::sql_insert_tuple(StateResult& res, ContingencyTable& ct){

    std::string query = "INSERT INTO RESULTS VALUES (";
    query += std::to_string(ct.a0) + ", ";  
    query += std::to_string(ct.a1) + ", ";  
    query += std::to_string(ct.b0) + ", ";  
    query += std::to_string(ct.b1) + ", ";  
    query += std::to_string(res.block_size) + ", ";  
    query += std::to_string(res.a_allocation) + ", "; 

    for(unsigned int i=0; i < n_attr - 1; ++i){
        query += fl_to_str(res.values[i]) + ", ";
    }
    query += fl_to_str(res.values[n_attr-1]);
    query += ");\n";
 
    return query;
}


