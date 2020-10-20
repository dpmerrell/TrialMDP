// block_rar_opt.cpp
// (c) 2020 David Merrell
//
// Implementation of BlockRAROpt class

#include "block_rar_opt.h"
#include "block_rar_table.h"
#include "state_result.h"
#include "transition_iterator.h"
#include "state_iterator.h"
#include "test_statistic.h"
#include <iostream>
#include <cstring>
#include <string>
#include <sqlite3.h>
#include <cmath>
#include <limits>


/**
 * The reward assigned to terminal states
 */
StateResult BlockRAROpt::terminal_reward(ContingencyTable ct){
    
    // some useful row sums:
    float N_a = ct.a0 + ct.a1;
    float N_b = ct.b0 + ct.b1;
    float N = N_a + N_b;
    float p_a = 0.5;
    if (N_a != 0.0){
        p_a = float(ct.a1) / N_a;
    } 
    float p_b = 0.5;
    if (N_b != 0.0){
        p_b = float(ct.b1) / N_b;
    }
    float P = 0.5;
    if (N != 0.0){
        P = float(ct.a1 + ct.b1) / N;
    }

    // Compute statistical power
    float power = (*test_statistic)(p_a, p_b, P, N_a, N_b, N);

    // Compute the "failures": the expected number of patients
    // that would have received a positive outcome, had they 
    // been given the superior treatment.
    float err;
    if(p_a >= p_b){
        err = (p_a - p_b)*N_b;
    } else{
        err = (p_b - p_a)*N_a;
    }

    // Compute the linear combination of those
    // factors
    float rwd = power - error_cost*err; // - block_cost*remaining_blocks;
                                        // ^^^This is zero for terminal states
    
    StateResult result = StateResult {0, 0, power, err, 0.0, rwd};

    return result;
}


/**
 * For a given state, find the action that maximizes
 * expected reward. Return the result (including the 
 * value of the maximized reward, and the terms of the
 * objective function)
 */
StateResult BlockRAROpt::max_expected_reward(int cur_idx, ContingencyTable ct){

    float FLOAT_NEG_INF = -std::numeric_limits<float>::infinity();
   
    // Track the best action we've seen thus far
    StateResult best_choice = StateResult {-1,-1,-1.0,-1.0,-1.0, FLOAT_NEG_INF};

    // Iterate through the possible actions
    action_iterator->reset(cur_idx);
    while (action_iterator->not_finished()){

        // Get the size index of the resulting contingency table
	int result_size_idx = action_iterator->get_block_size_idx();

        float prob = 0.0;
        // Initialize expected reward:
	float exp_power = 0.0;
	float exp_err = 0.0;
	float exp_blocks = 0.0;
	float exp_reward = 0.0;

	// Compute the expected reward for this action,
	// w.r.t. the randomness of the transition
	TransitionIterator tr_it = TransitionIterator(ct, action_iterator->action_a(),
			                                  action_iterator->action_b(),
                                                          prior_p, prior_strength);
	while(tr_it.not_finished()){

	    // Get the result struct associated with this state
            StateResult tr_res = (*results_table)(result_size_idx, tr_it.value());

	    // Get the probability of this transition
	    // and update the expected values:
	    prob = tr_it.prob();

            exp_power += (prob * tr_res.statistical_power);
	    exp_err += (prob * tr_res.n_failures);
	    exp_blocks += prob * tr_res.remaining_blocks;
	    exp_reward += prob * tr_res.reward;

            tr_it.advance();
        }

	// Account for the cost of this block:
	exp_blocks += 1.0;
	exp_reward -= block_cost;

	// Compare expected reward vs. best_choice
	if (exp_reward > best_choice.reward){
            best_choice.next_block_size = action_iterator->get_block_size();
	    best_choice.next_a_allocation = action_iterator->action_a();
	    best_choice.statistical_power = exp_power;
	    best_choice.n_failures = exp_err;
	    best_choice.remaining_blocks = exp_blocks;
	    best_choice.reward = exp_reward; 
	}

        action_iterator->advance();
    
    }
    // Return the best action (and corresponding reward)
    return best_choice;
}


// Constructor
BlockRAROpt::BlockRAROpt(int n_p, int b_i, float e_c, float b_c, float pr_p, float pr_s, std::string ts){

    n_patients = n_p;
    block_incr = b_i;
    error_cost = e_c;
    block_cost = b_c;
    prior_p = pr_p;
    prior_strength = pr_s;

    if (ts == "wald"){
      test_statistic = new WaldStatistic();
    }else{
      std::cerr << ts << " not a valid value for test statistic." << std::endl;
      throw(1);
    }

    results_table = new BlockRARTable(n_p, b_i); 
    state_iterator = new StateIterator(*(results_table)); 
    action_iterator = new ActionIterator(0.2, 0.8, 7, 
		                         results_table->get_n_vec(),
                                         0);
}


void BlockRAROpt::solve(){

    // Iterate through the terminal states;
    // set the terminal rewards
    int terminal_idx = state_iterator->get_cur_idx();
    int cur_idx = terminal_idx;
    ContingencyTable cur_table; 
    cur_table = state_iterator->value();

    while(cur_idx == terminal_idx){
	
	(*results_table)(terminal_idx, cur_table) = terminal_reward(cur_table);
	
	state_iterator->advance();
	cur_idx = state_iterator->get_cur_idx();
	cur_table = state_iterator->value();

    }
 
    // Move on to the earlier states. 
    // compute the maximal action for each one.
    while(state_iterator->not_finished()){

        cur_table = state_iterator->value();
        (*results_table)(cur_idx, cur_table) = max_expected_reward(cur_idx, cur_table);

        state_iterator->advance();
	cur_idx = state_iterator->get_cur_idx();
    }
    std::cout << "FIRST MOVE:" << std::endl;
    StateResult first_move = (*results_table)(0, cur_table);

    std::cout << "\tblock size: " << first_move.next_block_size << std::endl;
    std::cout << "\t\tN_A: " << first_move.next_a_allocation << std::endl;
    std::cout << "\t\tN_B: " << first_move.next_block_size - first_move.next_a_allocation << std::endl;

    std::cout << "\nEXPECTED REWARD:" << std::endl;
    std::cout << "\tWald statistic: " << first_move.statistical_power << std::endl; 
    std::cout << "\tFailures: " << first_move.n_failures << std::endl; 
    std::cout << "\tNumber of blocks: " << first_move.remaining_blocks << std::endl;
    std::cout << "\n\tTotal: " << first_move.reward << std::endl;

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

void BlockRAROpt::to_sqlite(char* db_fname, int chunk_size=10000){

    // Make database connection pointer
    sqlite3* db;

    try{
        // Connect to database
        int conn_result = 1;
        conn_result = sqlite3_open(db_fname, &db);
        if(conn_result != 0){ throw 1; }

        // Build table in database
	std::string build_expr = "CREATE TABLE RESULTS("\
            	              	"A0 INT, A1 INT, B0 INT, B1 INT, "\
				"BlockSize INT, AAllocation INT, "\
				"TestStatistic REAL, Failures REAL, RemainingBlocks REAL, Reward REAL, "\
            	               	"PRIMARY KEY (A0, A1, B0, B1)"\
            	               	");";
        int build_result = 1;
        build_result = sqlite3_exec(db, build_expr.c_str(), NULL, NULL, NULL);
        if (build_result != 0){ throw 2; }

	//
	std::string transaction_start = "BEGIN TRANSACTION;\n";
	std::string line_start = "INSERT INTO RESULTS VALUES (";
        std::string transaction_end = " COMMIT;";

	// Iterate over the results
	StateIterator result_iter = StateIterator(*(results_table));
        while(result_iter.not_finished()){

	    // Prepare a chunk of INSERTs...
	    std::string insert_expr = transaction_start;
            int chunk_idx = 0;

	    while(result_iter.not_finished() && chunk_idx < chunk_size){

		// Get the contingency table and corresponding results
                ContingencyTable cur_table = result_iter.value();
	        int cur_idx = result_iter.get_cur_idx();
                StateResult cur_results = (*results_table)(cur_idx, cur_table);
	                
	        // Add a line to the transaction
		insert_expr += line_start;
		insert_expr += std::to_string(cur_table.a0) + ", "; 
		insert_expr += std::to_string(cur_table.a1) + ", "; 
		insert_expr += std::to_string(cur_table.b0) + ", "; 
		insert_expr += std::to_string(cur_table.b1) + ", "; 
		insert_expr += std::to_string(cur_results.next_block_size) + ", "; 
		insert_expr += std::to_string(cur_results.next_a_allocation) + ", "; 
		insert_expr += fl_to_str(cur_results.statistical_power) + ", "; 
		insert_expr += std::to_string(cur_results.n_failures) + ", "; 
		insert_expr += std::to_string(cur_results.remaining_blocks) + ", "; 
		insert_expr += fl_to_str(cur_results.reward) + ");\n"; 

		// Move to the next result
                result_iter.advance();

	    }
	    insert_expr += transaction_end;

	    // INSERT the chunk
            int insert_result = 1;
            insert_result = sqlite3_exec(db, insert_expr.c_str(), NULL, NULL, NULL);
            if (insert_result != 0){ throw 3; }
	}
        // Close connection
        sqlite3_close(db);

    }
    catch(int code){ 
        switch(code){
	    case 1:
	        std::cerr << "`to_sqlite`: failed to connect to SQLite database at location " << db_fname << std::endl;
		break;
	    case 2:
		std::cerr << "`to_sqlite`: failed to build table RESULTS in database." << std::endl; 
		break;
	    case 3: 
		std::cerr << "`to_sqlite`: failed to insert rows into table RESULTS." << std::endl; 
		break;
	    default: 
		std::cerr << "`to_sqlite`: method failed." << std::endl; 
		break;
        }
    }

}
    

BlockRAROpt::~BlockRAROpt(){
    delete state_iterator;
    delete action_iterator;
    delete results_table;
    delete test_statistic;
}
