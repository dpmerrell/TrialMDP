// r_interface.cpp
// (c) 2020 David Merrell
//

#include "block_rar_opt.h"
#include <string>
#include <iostream>
#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::depends(BH)]]

// [[Rcpp::plugins("cpp11")]]

// [[Rcpp::export]]
void block_rar_opt(int n_patients, int block_incr,
                   float failure_cost, float block_cost,
                   std::string sqlite_fname,
                   float prior_a0 = 1.0,
                   float prior_a1 = 1.0,
                   float prior_b0 = 1.0,
                   float prior_b1 = 1.0,
                   std::string transition_dist="beta_binom",
                   std::string test_statistic="wald",
                   float act_l=0.2, float act_u=0.8, int act_n=7) {


  BlockRAROpt solver = BlockRAROpt(n_patients, block_incr,
                                   failure_cost, block_cost,
                                   prior_a0, prior_a1,
                                   prior_b0, prior_b1,
                                   transition_dist,
                                   test_statistic,
                                   act_l, act_u, act_n);
  
  std::cout << "Solver initialized." << std::endl;
  std::cout << "\tN patients: " << n_patients << std::endl; 
  std::cout << "\tBlock increment: " << block_incr << std::endl;
  std::cout << "\tAllocations: {" << act_l << ", ..., " << act_u << "} (" << act_n << ")" << std::endl; 
  std::cout << "\tFailure cost: " << failure_cost << std::endl; 
  std::cout << "\tBlock cost: " << block_cost << std::endl; 
  std::cout << "\tTest statistic: " << test_statistic << std::endl; 
  std::cout << "Solving." << std::endl;
  
  solver.solve();
  std::cout << "Solver completed." << std::endl;
  
  char* fname = new char[sqlite_fname.length() + 1];
  strcpy(fname, sqlite_fname.c_str());
  
  solver.to_sqlite(fname, 100);
  std::cout << "Saved to file: " << sqlite_fname << std::endl;
  
  delete[] fname;
  
}


