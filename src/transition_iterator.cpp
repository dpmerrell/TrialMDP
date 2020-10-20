// transition_iterator.cpp
// (c) 2020 David Merrell
//

#define __STDCPP_WANT_MATH_SPEC_FUNCS__ 1

#include "contingency_iterator.h"
#include "contingency_table.h"
#include "transition_iterator.h"
#include <cmath>

float binom(int n, int k){
    return 1.0/((n+1.0)*std::beta(n-k+1,k+1));
}

float binom_prob(int N, float p, int x){
    return binom(N,x) * pow(p,x) * pow(1.0 - p, N-x);
}

std::vector<float> initialize_probs(int N, float p){
    std::vector<float> probs = std::vector<float>(N+1, 0);
    for(int i=0; i < N+1; i++){
        probs[i] = binom_prob(N, p, i);
    }
    return probs;
}

TransitionIterator::TransitionIterator(ContingencyTable ct, int size_a, int size_b, float pr_p, float pr_s){

    cur_table = ct;

    a_size = size_a;
    b_size = size_b;

    a_counter = 0;
    b_counter = 0;

    float smoothing = pr_p*pr_s;

    float p_a = (float(cur_table.a1) + smoothing) / (float(cur_table.a0 + cur_table.a1) + pr_s);
    a_probs = initialize_probs(a_size, p_a);

    float p_b = (float(cur_table.b1) + smoothing) / (float(cur_table.b0 + cur_table.b1) + pr_s);
    b_probs = initialize_probs(b_size, p_b);

}


ContingencyTable TransitionIterator::value(){
    short unsigned int a0 = cur_table.a0 + a_size - a_counter;
    short unsigned int a1 = cur_table.a1 + a_counter;
    short unsigned int b0 = cur_table.b0 + b_size - b_counter;
    short unsigned int b1 = cur_table.b1 + b_counter;
    return ContingencyTable(a0, a1, b0, b1);

}


float TransitionIterator::prob(){
    return a_probs[a_counter] * b_probs[b_counter];
}


bool TransitionIterator::not_finished(){
    return (a_counter <= a_size);
}


void TransitionIterator::advance(){
    if (b_counter < b_size){
        b_counter++;
    }
    else{
        a_counter++;
        b_counter = 0;
    }
}


