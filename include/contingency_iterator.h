// contingency_iterator.h
// (c) 2020-08 David Merrell
//
// Definition of a class that iterates over
// contingency tables of a given size.
//

#ifndef _CONT_ITER_H
#define _CONT_ITER_H

#include "contingency_table.h"
#include <vector>

class SimplexIterator {

    private:
	// private data
	int dim;
	int total;
	SimplexIterator* sub_state;

	// private methods
	void increment_total(){ total++; cur_value[0]++; }
	void restart();
        void reset_totals();	

    public:
	// public data member
	int* cur_value;

        // constructors
	SimplexIterator();
	SimplexIterator(int, int, int*);

	// copy constructor
        SimplexIterator(const SimplexIterator& it);

	// Assignment operator
	SimplexIterator& operator=(SimplexIterator);

	// other public methods
	void set_sub(SimplexIterator* sub_iter);
	bool not_finished();
	void advance();
	void pretty_print();
    
};


class ContingencyIterator {

	private:
	    std::vector<SimplexIterator*> state;
	    std::vector<SimplexIterator*> simplex_iter_factory(int dim, int total, int* memloc);

	public:
	    ContingencyIterator(int size){
		int* cur_value;
	        cur_value = new int [4];
	        state = this->simplex_iter_factory(4, size, cur_value); 
	    }

	    bool not_finished(){ return state[0]->not_finished(); }
	    
	    ContingencyTable value(){ 
		    ContingencyTable result;
		    result.a0 = state[0]->cur_value[0];  
		    result.a1 = state[0]->cur_value[1];  
		    result.b0 = state[0]->cur_value[2];  
		    result.b1 = state[0]->cur_value[3];  
		    return result;
	    }
	    void advance(){ state[0]->advance(); }

	    ~ContingencyIterator(){
                delete state[0]->cur_value;
		for(unsigned int i=0; i<state.size(); i++){
                    delete state[i];
		}
	    }

	    void pretty_print();
};

#endif
