//
//  prime.cpp
//  prime_calc
//
//  Created by Arun Mavumkal on 11/29/16.
//  Copyright © 2016 Arun Mavumkal. All rights reserved.
//

#include "prime.hpp"
#include <pthread.h>

Factor::Factor(long intA) {
    numToFactor = intA;
}

void Factor::factorInt() {
    unsigned long i = 1;
    if (numToFactor == 0) {
        return;
    }
    while(true) {
        if (numToFactor % i == 0) {
            if (i > 1) {
                isPrime = false;
                return;
            }
            unsigned long result = numToFactor/i;
            leftFactorValue.push_back(i);
            rightFactorValue.push_back(result);
            if (i == result) break;
        }
        if (i*i > numToFactor) break;
        ++i; 
    }
    if (*leftFactorValue.end() == 1) isPrime = true;
}
