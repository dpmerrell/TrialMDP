

#include "contingency_iterator.h"
#include <iostream>



void SimplexIterator::set_sub(SimplexIterator* sub_iter){
    state = sub_iter;
}

bool SimplexIterator::not_finished(){
    return !(cur_value[dim-1] == total);
}


void SimplexIterator::restart(){
    if(dim > 1){
        cur_value[0] = total;
        cur_value[dim-1] = 0;
    }
}


void SimplexIterator::advance(){

    if(dim == 1){
        return;
    }
    else{
        if(state->not_finished()){
            state->advance();
	}
	else{
            state->increment_total();
            state->restart();
            cur_value[0] -= 1;
	}
    }
}


void SimplexIterator::pretty_print(){

    
    std::cout << "ADDR: " << this << "\tDIM: " << dim << "\tTOTAL: " << total << "\tCHILD: " << state << std::endl;
    if (state != NULL){
        state->pretty_print();
    }

    //std::cout << "[";
    //for(unsigned int i=0; i < dim - 1; i++){
    //    std::cout << cur_value[i] << ", ";
    //}
    //std::cout << cur_value[dim-1] << std::endl;

}


void ContingencyIterator::pretty_print(){
    state->pretty_print(); 
}


SimplexIterator* ContingencyIterator::simplex_iter_factory(int dim, int total, int* memloc){
    SimplexIterator* it_stack;
    it_stack = new SimplexIterator [dim];
    it_stack[0] = SimplexIterator(dim, total, memloc);
    it_stack[0].cur_value[0] = total;

    for(int i = 1; i < dim; i++){
        it_stack[i] = SimplexIterator(dim - i, 0, memloc + i);
	it_stack[i-1].set_sub(it_stack+i);
	memloc[i] = 0;
    }

    return it_stack; 
}
