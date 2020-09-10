// block_rar_opt.cpp
//
// Implementation of BlockRAROpt class

#include "block_rar_opt.h"
#include "block_rar_table.h"
#include "state_result.h"
#include "transition_iterator.h"
#include "state_iterator.h"
#include <cmath>
#include <limits>



float statistical_power(float p_a, float p_b, float P, float N_a, float N_b, float N){

    // The Wald statistic
    float W = ( pow(p_a - p_b, 2.0) / (P * (1.0 - P)) ) * N_a*N_b / N;
    // (is asymptotically chi-square(df=1) under null hypothesis)

    // For now, just return the Wald statistic.
    // We probably ought to return a different quantity
    // that allows us to talk more naturally about tradeoffs between
    // statistical power and other terms of the objective--
    // some monotone transformation of W.
    // (e.g., an exceedence probability?)
    return W;   
}


/**
 * The reward assigned to terminal states
 */
StateResult BlockRAROpt::terminal_reward(ContingencyTable ct){
    
    // some useful row sums:
    float N_a = ct.a0 + ct.a1;
    float N_b = ct.b0 + ct.b1;
    float N = N_a + N_b;
    float p_a = float(ct.a1) / N_a;
    float p_b = float(ct.b1) / N_b;
    float P = float(ct.a1 + ct.b1) / N;

    // Compute statistical power
    float power = statistical_power(p_a, p_b, P, N_a, N_b, N);

    // Compute the "error": the expected number of patients
    // that would have received a positive outcome, had they 
    // been given the superior treatment.
    float err;
    if(p_a > p_b){
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
    action_iterator.reset(cur_idx);
    while (action_iterator.not_finished()){

        // Get the size index of the resulting contingency table
	int result_size_idx = action_iterator.get_cur_size_idx();

	float prob = 0.0;
        // Initialize expected reward:
	float exp_power = 0.0;
	float exp_err = 0.0;
	float exp_blocks = 0.0;
	float exp_reward = 0.0;

	// Iterate through the possible outcomes
	TransitionIterator tr_it = TransitionIterator(ct, action_iterator.action_a(),
			                                  action_iterator.action_b());
	while(tr_it.not_finished()){

	    // Get the result struct associated with this state
            StateResult tr_res = results_table(result_size_idx, tr_it.value());

	    // Get the probability of this transition
	    // and update the expected values:
	    prob = tr_it.prob(); 
            exp_power += prob * tr_res.statistical_power;
	    exp_err += prob * tr_res.n_failures;
	    exp_blocks += prob * tr_res.remaining_blocks;
	    exp_reward += prob * tr_res.reward;

            tr_it.advance();
        }

	///////////////////////////////////////
	// (DON'T MISS THE LAST ONE...)
	// Get the result struct associated with this state
        StateResult tr_res = results_table(result_size_idx, tr_it.value());

	// Get the probability of this transition
	// and update the expected values:
	prob = tr_it.prob(); 
        exp_power += prob * tr_res.statistical_power;
	exp_err += prob * tr_res.n_failures;
	exp_blocks += prob * tr_res.remaining_blocks;
	exp_reward += prob * tr_res.reward;
        /////////////////////////////////////////


	// Account for the cost of this block:
	exp_blocks += 1.0;
	exp_reward -= block_cost;

	// Compare expected reward vs. best_choice
	if (exp_reward > best_choice.reward){
            best_choice.next_block_size = action_iterator.get_block_size();
	    best_choice.next_a_allocation = action_iterator.action_a();
	    best_choice.statistical_power = exp_power;
	    best_choice.n_failures = exp_err;
	    best_choice.remaining_blocks = exp_blocks;
	    best_choice.reward = exp_reward; 
	}
    
    }
    // Return the best action (and corresponding reward)
    return best_choice;
}


// Constructor
BlockRAROpt::BlockRAROpt(int n_p, int b_i, float e_c, float b_c){

    std::cout << "\tSUCCESSFULLY BEGAN CONSTRUCTOR" << std::endl;
    n_patients = n_p;
    block_incr = b_i;
    error_cost = e_c;
    block_cost = b_c;
    std::cout << "\tSUCCESSFULLY ASSIGNED DUMB PARAMS" << std::endl;

    results_table = BlockRARTable(n_p, b_i); 
    std::cout << "\tSUCCESSFULLY BUILT RESULTS TABLE" << std::endl;
    state_iterator = StateIterator(results_table); 
    std::cout << "\tSUCCESSFULLY BUILT STATE ITERATOR" << std::endl;
    action_iterator = ActionIterator(0.0, 1.0, 11, 
		                     results_table.get_n_vec(),
				     0);
    std::cout << "\tSUCCESSFULLY BUILT ACTION ITERATOR" << std::endl;
}


void BlockRAROpt::solve(){

    // Iterate through the terminal states;
    // set the terminal rewards
    int terminal_idx = state_iterator.get_cur_idx();
    int cur_idx = terminal_idx;
    ContingencyTable cur_table = state_iterator.value();
    while(cur_idx == terminal_idx){
	
	results_table(terminal_idx, cur_table) = terminal_reward(cur_table);
        
	state_iterator.advance();
	cur_idx = state_iterator.get_cur_idx();
	cur_table = state_iterator.value();
    }
 
    // Move on to the earlier states. 
    // compute the maximal action for each one.
    while(state_iterator.not_finished()){

        results_table(cur_idx, cur_table) = max_expected_reward(cur_idx, cur_table);

        state_iterator.advance();
	cur_idx = state_iterator.get_cur_idx();
        cur_table = state_iterator.value();
    }
    results_table(cur_idx, cur_table) = max_expected_reward(cur_idx, cur_table);

}


void BlockRAROpt::save_results(std::string out_filename){
    return;
}
