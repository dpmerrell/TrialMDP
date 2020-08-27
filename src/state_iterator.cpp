// state_iterator.cpp
// (c) 2020-08 David Merrell
//
// Implementation of StateIterator class

#include "state_iterator.h"
#include <vector>
#include "contingency_iterator.h"
#include "contingency_table.h"
#include "block_rar_table.h"
#include <iostream>


StateIterator::StateIterator(BlockRARTable tab){
    n_vec = tab.get_n_vec();
    cur_idx = n_vec.size()-1;
    cur_iter = new ContingencyIterator(n_vec[cur_idx]);
    cur_count = 1;
}


ContingencyTable StateIterator::value(){
    return cur_iter->value();
}


void StateIterator::advance(){
    if(cur_iter->not_finished()){
        cur_iter->advance();
        cur_count++;
    }
    else{
        --cur_idx;
	if (cur_idx >= 0){
            delete cur_iter;
            cur_iter = new ContingencyIterator(n_vec[cur_idx]);
            cur_count = 1;
	}
    }
}

int StateIterator::get_cur_idx(){
    return cur_idx;
}

bool StateIterator::not_finished(){
    return (cur_idx >= 0);
}	


StateIterator::~StateIterator(){
    delete cur_iter;
}
