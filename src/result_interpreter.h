// result_interpreter.h
// (c) 2021-02 David Merrell
//
// The ResultInterpreter class assigns meaning
// to the fields in a StateResult struct.
// This allows a StateResult to be, essentially, 
// a static array of unlabeled floating point numbers.
// This keeps StateResult as lightweight as
// possible, so the BlockRAROpt instance can store
// an enormous table of them. 
// 

#ifndef __RESULT_INTERP_H_
#define __RESULT_INTERP_H_

#include "contingency_table.h"
#include "lookahead_rule.h"
#include "state_result.h"
#include <vector>
#include <string>
#include <unordered_map>

class ResultInterpreter{

    private:
        
        int n_attr;
        std::vector< std::string > attr_names;
        std::unordered_map<std::string, int> attr_to_idx;
        std::vector< LookaheadRule* > lookahead_rules;
        std::vector< float > lookahead_values;
        
        // Private method (initializes attr_to_idx) 
        std::unordered_map<std::string, int> make_dict(std::vector< std::string > names);

    public:

        ResultInterpreter(){ 
            n_attr = 0;
            attr_names = std::vector< std::string >();
            attr_to_idx = std::unordered_map< std::string, int>();
            lookahead_rules = std::vector< LookaheadRule* >();
            lookahead_values = std::vector< float >();
        }

        ResultInterpreter(std::string test_statistic,
                          float failure_cost,
                          float block_cost, int n_pat);

        float get_attr(StateResult& res, std::string attr_name); 
        void set_attr(StateResult& res, std::string attr_name, float new_value); 

        int get_n_attr(){ return n_attr; }

        std::string name_from_idx(int idx){ return attr_names[idx]; }

        std::string pretty_print_result(StateResult& res);

        // Functions for saving results to a SQLite database
        std::string sql_create_table();
        std::string sql_insert_tuple(StateResult& res, ContingencyTable& ct);

        // These functions encode how we compute results for
        // this state from the results of a future state; 
        // i.e., how the dynamic programming algorithm "looks ahead"
        // to future states
        void compute_lookaheads(ContingencyTable& current_state, int a_A, int a_B, int n_A, int n_B, StateResult& next_state);
        void clear_lookaheads();
        float look_ahead(int idx);

};

#endif
