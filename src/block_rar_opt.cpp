// block_rar_opt.cpp
//
// Implementation of BlockRAROpt class

#include "block_rar_opt.h"
#include "block_rar_table.h"
#include <cmath>


float statistical_power(float p_a, float p_b, float P){

    // The Wald statistic
    float W = ( (p_a - p_b)^2 / (P * (1.0 - P) )) * N_a*N_b / (N_a + N_b)
    // (is asymptotically chi-square(df=1) under null hypothesis)

    // For now, just return the Wald statistic.
    // We probably ought to return a different quantity
    // that allows us to talk more naturally about tradeoffs between
    // statistical power and other terms of the objective.
    // (e.g., an exceedence probability?)
    return W;   
}

/**
 * The reward assigned to terminal states
 */
StateResult BlockRAROpt::reward(ContingencyTable ct){
    
    // some useful row sums:
    float N_a = ct.a0 + ct.a1;
    float N_b = ct.b0 + ct.b1;
    float N = N_a + n_b;
    float p_a = float(ct.a1) / N_a;
    float p_b = float(ct.b1) / N_b;
    float P = float(ct.a1 + ct.b1) / N;

    // Compute statistical power
    float power = statistical_power(p_a, p_b, P);

    // Compute the "error": the expected number of patients
    // that would have received a positive outcome, had they 
    // been given the superior treatment.
    float err;
    if(p_a > p_b){
        err = p_a*N - p_b*N_b;
    } else{
        err = p_b*N - p_a*N_a;
    }

    // Compute the remaining blocks (0 for these terminal blocks)
    float remaining_blocks = 0.0;

    // Compute the linear combination of those
    // factors
    return power - error_cost*err - block_cost*remaining_blocks;
}


StateResult BlockRAROpt::max_expected_reward(ContingencyTable ct){

}


// Constructor
BlockRAROpt::BlockRAROpt(int n_p, int b_i, float e_c, float b_c){

    num_patients = n_p;
    block_incr = b_i;
    error_cost = e_c;
    block_cost = b_c;

    results_table = BlockRARTable(n_p, b_i); 

    state_iterator = StateIterator(results_table); 

}


void BlockRAROpt::solve(){

}


void BlockRAROpt::save_results(string out_filename){

}
