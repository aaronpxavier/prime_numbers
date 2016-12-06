//
//  prime.cpp
//  prime_calc
//
//  Created by Arun Mavumkal on 11/29/16.
//  Copyright © 2016 Arun Mavumkal. All rights reserved.
//

#include "prime.hpp"
#include <stdexcept>

//pre: INT_A must be a positive integer
//post: Factor object created
Factor::Factor(const long INT_A) {
    if (INT_A < 1) {
        throw std::invalid_argument("must pass in positive integer");
    }
    this->NUM_TO_FACTOR = INT_A;
}

//pre:: method takes no arguments
//post:: finds all postive factors for number stored in NUM_TO_FACTOR
void Factor::factor_int() {
    unsigned long i = 1;
    if (this->NUM_TO_FACTOR == 0) {
        return;
    }
    while(true) {
        if (this->NUM_TO_FACTOR % i == 0) {
            unsigned long result = this->NUM_TO_FACTOR/i;
            left_factor_value.push_back(i);
            right_factor_value.push_back(result);
            if (i == result) break;
        }
        if (i*i > this->NUM_TO_FACTOR) break;
        ++i; 
    }
    if (*left_factor_value.end() == 1) is_prime = true;
}

bool Factor::check_is_prime() {
    unsigned long i = 1;
    if (this->NUM_TO_FACTOR == 0) {
        return false;
    }
    while(true) {
        if (this->NUM_TO_FACTOR % i == 0) {
            if (i > 1) {
                is_prime = false;
                return false;
            }
            unsigned long result = this->NUM_TO_FACTOR/i;
            left_factor_value.push_back(i);
            right_factor_value.push_back(result);
            if(i == result) break;
        }
        if (i*i > this->NUM_TO_FACTOR) break;
        ++i;
    }
    if (*left_factor_value.end() == 1) is_prime = true;
    return true;
}
