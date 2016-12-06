//
//  prime.cpp
//  prime_calc
//
//  Created by Arun Mavumkal on 11/29/16.
//  Copyright © 2016 Arun Mavumkal. All rights reserved.
//

#include "prime.hpp"
#include <stdexcept>
#include <iostream>

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
    
    if (this->is_prime) {
        left_factor_value.push_back(this->NUM_TO_FACTOR);
        right_factor_value.push_back(1);
        return;
    }
    
    if (this->NUM_TO_FACTOR == 0)
        return;
    
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

//pre: function takes no arguments, factor_int() must be called first
//post: prints out positive factors for NUM_TO_FACTOR
void Factor::print_factors() {
    if (left_factor_value.empty())
        throw std::runtime_error("print_factors fn called befor factor_int() factors lists are empty");
    std::cout << "\npositive factors for: " << this->NUM_TO_FACTOR << std::endl;
    for (std::list<unsigned long>::iterator left_itr = left_factor_value.begin(), right_itr = right_factor_value.begin(); left_itr != left_factor_value.end(); ++left_itr, ++right_itr) {
        std::cout << *left_itr << " * " << *right_itr << std::endl;
    }
}

bool Factor::check_is_prime() {
    unsigned long i = 1;
    if (this->NUM_TO_FACTOR == 0) {
        return false;
    }
    while(i*i < this->NUM_TO_FACTOR) {
        if (this->NUM_TO_FACTOR % i == 0) {
            if (i > 1) {
                is_prime = false;
                return false;
            }
        }
        ++i;
    }
    this->is_prime = true;
    return true;
}
