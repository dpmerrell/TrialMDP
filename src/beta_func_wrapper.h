// beta_func_wrapper.h
// (c) David Merrell 2021-01
//
// We need to call the Beta function many times.
// But we only ever use it on a discrete set of arguments.
// So it behooves us to memoize the function.

#ifndef _BETA_FUNC_WRAPPER_H
#define _BETA_FUNC_WRAPPER_H


#define __STDCPP_WANT_MATH_SPEC_FUNCS__ 1

#include <cmath>
#include <vector>

class BetaFuncWrapper {

    private:
        float a_offset;
        float b_offset;
        std::size_t N;
        std::vector< std::vector<float> > output_cache;

    public:

        // Constructor
        BetaFuncWrapper(float a_off, float b_off, std::size_t max_arg){ 
            a_offset = a_off;
            b_offset = b_off;
            N = max_arg;
            output_cache = std::vector< std::vector<float> >(N, std::vector<float>(-1.0, N));
            return; 
        }

        // Call the (memoized) Beta function
        float operator() (int a, int b){
            if(output_cache[a][b] > 0.0){
                return output_cache[a][b]
            }
            output_cache[a][b] = std::beta(a_offset + a, b_offset + b);
            return output_cache[a][b];
        }

};

#endif
