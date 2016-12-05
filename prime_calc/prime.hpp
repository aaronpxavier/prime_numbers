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
    void factorInt ();
    bool isPrime = false;
    
private:
    std::list<unsigned long> leftFactorValue;
    std::list<unsigned long> rightFactorValue;
    
    unsigned long numToFactor = 0;
};

#endif /* prime_hpp */
