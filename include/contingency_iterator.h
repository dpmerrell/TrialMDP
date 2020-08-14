// contingency_iterator.h
// (c) 2020-08 David Merrell
//
// Definition of a class that iterates over
// contingency tables of a given size.
//

#ifndef _CONT_ITER_H
#define _CONT_ITER_H

#include "contingency_table.h"

class SimplexIterator {

    private:
	// private data
	int dim;
	int total;
	SimplexIterator* state;

	// private methods
	void increment_total(){ total++; }
	void restart();	

    public:
	// public data member
	int* cur_value;

        // constructors
	SimplexIterator(){
            dim = 0;
	    total = 0;
	    cur_value = NULL;
	    state = NULL;
	}

	SimplexIterator(int dim, int total, int* memloc){
            dim = dim;
            total = total;
            cur_value = memloc;
            state = NULL;
        }

	// public methods
	void set_sub(SimplexIterator* sub_iter);
	bool not_finished();
	void advance();
	void pretty_print();
    
};




class ContingencyIterator {

	private:
            SimplexIterator* state;
	    SimplexIterator* simplex_iter_factory(int dim, int total, int* memloc);

	public:
	    ContingencyIterator(int size){
		int* cur_value;
	        cur_value = new int [4];
	        state = this->simplex_iter_factory(4, size, cur_value); 
	    }

	    bool not_finished(){ return state->not_finished(); }
	    
	    ContingencyTable value(){ 
		    ContingencyTable result;
		    result.a0 = state->cur_value[0];  
		    result.a1 = state->cur_value[1];  
		    result.b0 = state->cur_value[2];  
		    result.b1 = state->cur_value[3];  
		    return result;
	    }
	    void advance(){ state->advance(); }

	    ~ContingencyIterator(){
                delete state->cur_value;
		delete state;
	    }

	    void pretty_print();
};

#endif
