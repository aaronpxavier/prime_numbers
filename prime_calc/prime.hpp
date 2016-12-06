//
//  prime.hpp
//  prime_calc
//
//  Created by Arun Mavumkal on 11/29/16.
//  Copyright © 2016 Arun Mavumkal. All rights reserved.
//

#ifndef prime_hpp
#define prime_hpp
#include <stdio.h>
#include <list>



class Factor {

public:
    Factor(long intA);
    void factor_int();
    void print_factors(); 
    bool check_is_prime();
    bool is_prime = false;
    
private:
    std::list<unsigned long> left_factor_value;
    std::list<unsigned long> right_factor_value;
    
    unsigned long NUM_TO_FACTOR;
};

#endif /* prime_hpp */
