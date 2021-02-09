// contingency_table.h
// (c) 2020-08 David Merrell
//
// A simple struct definition for 2x2 contingency tables.

#ifndef _CONT_TABLE_H
#define _CONT_TABLE_H

#include <iostream>
#include <string>

struct ContingencyTable {
    short unsigned int a0;
    short unsigned int a1;
    short unsigned int b0;
    short unsigned int b1;

    ContingencyTable(){
        a0 = 0;
	a1 = 0;
	b0 = 0;
	b1 = 0;
    }

    ContingencyTable(int other_a0, int other_a1, int other_b0, int other_b1){
        a0 = other_a0;
	a1 = other_a1;
	b0 = other_b0;
	b1 = other_b1;
    }

    ContingencyTable(const ContingencyTable& other){
        a0 = other.a0;
	a1 = other.a1;
	b0 = other.b0;
	b1 = other.b1;
    }

    void pretty_print(){
        std::cout << a0 << "\t" << a1 << std::endl;
	std::cout << b0 << "\t" << b1 << std::endl;
    }

    bool operator==(const ContingencyTable& other) const {
	return (a0 == other.a0) && (a1 == other.a1) && (b0 == other.b0) && (b1 == other.b1);
    }

    ContingencyTable& operator+=(const ContingencyTable& other){
        a0 += other.a0;
	a1 += other.a1;
	b0 += other.b0;
	b1 += other.b1;
	return *this;
    }

};



struct CTHash {

    int N = 1000;
    int N2 = 1000000;
    int N3 = 1000000000;

    std::size_t operator()(const ContingencyTable& t) const{
        return t.a0*N3 + t.a1*N2 + t.b0*N + t.b0;
    }

};

#endif

