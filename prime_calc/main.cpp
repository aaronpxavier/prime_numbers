//
//  main.cpp
//  prime_calc
//
//  Created by Arun Mavumkal on 11/29/16.
//  Copyright © 2016 Arun Mavumkal. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <future>
#include <list>
#include <mutex>
#include "prime.hpp"

const unsigned int MAX_THREADS = std::thread::hardware_concurrency();
unsigned long long largest_prime;
std::mutex LARGEST_PRIME_LOCK, CLOSE_PROGRAM_LOCK;

bool close_program = false;


//pre: fn takes no arguments.
//post: prints out the value of hte largest prime every 2 seconds.
void printLargestPrime() {
    while(!close_program) {
        LARGEST_PRIME_LOCK.lock();
        if (largest_prime != 0)
            std::cout << "the Largest prime is: " << largest_prime << std::endl;
        LARGEST_PRIME_LOCK.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
}

//pre integer_to_check must be declared and defined.
//post if number is prime and larger than largest_prime than largest_prime gets updated. No value returned.
void find_prime(const unsigned long long &integer_to_check) {
    Factor factor(integer_to_check);
    if(factor.check_is_prime())
    {
        LARGEST_PRIME_LOCK.lock();
        if (integer_to_check > largest_prime)
            largest_prime = integer_to_check;
        LARGEST_PRIME_LOCK.unlock();
    }
}


//pre: start and finish must be declared and defined with positive integers;
//post:functions ends when i reaches finish. looks for all prime numbers in interval by passing values into find_prime fn.
void findPrimeInInterval(unsigned long long start, const unsigned long FINISH) {
    
    if (start % 2 == 0)
        ++ start;
    
    for (unsigned long long i = start; i < FINISH; i+=2) {
        if (close_program) break;
        find_prime(i);
    }
}

//pre i and max_threads must be declared and defined with positive integers.
//post: no value returned. spawns threads to to calculate prime numbers in an interval.
void thread_spawner (unsigned long long start, unsigned int max_threads) {
    
    unsigned long long threads_interval_begin = start;
    std::future<void> *future;
    std::list<std::future<void>*> threads;
    
    // loops while program close command has not been given.
    while(!close_program) {
        if (threads.size() < max_threads) {
            try {
                future = new std::future<void>;
                // CREATES THREADS BREAKING PROBLEM INTO SECTIONS OF 3,000,000 EACH
                *future = std::async(findPrimeInInterval, threads_interval_begin, threads_interval_begin + 3000000L);
                threads_interval_begin += 3000000ULL;
                threads.push_back(future);
            }
            catch(std::system_error &e) {
                std::cout << "\n\nSystem error: " << e.what() << "\n\n";
                std::cout << &e;
            }
        }
        else {
            
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            
            for (std::list<std::future<void>*>::iterator itr = threads.begin(); itr != threads.end(); ++itr) {
                
                std::future<void> *thread = *itr;
                auto status = thread->wait_for(std::chrono::seconds(0));
                
                if (status == std::future_status::ready) {
                    delete *itr;
                    itr = threads.erase(itr);
                }
            }
        }
        
        start+=2;
    }
    while (threads.size() > 0)
        for (std::list<std::future<void>*>::iterator itr = threads.begin(); itr != threads.end(); ++itr) {
            std::future<void> *thread = *itr;
            auto status = thread->wait_for(std::chrono::seconds(0));
            if (status == std::future_status::ready) {
                delete *itr;
                itr = threads.erase(itr);
            }
        }

    
}

//pre: num_to_factor must be decalared and defined with positive integer.
void factor_number (const unsigned long long &num_to_factor) {
    
    Factor factor(num_to_factor);
    factor.factor_int();
    factor.print_factors();
    if (factor.is_prime) {
        LARGEST_PRIME_LOCK.lock();
        if(num_to_factor > largest_prime)
            largest_prime = num_to_factor;
        LARGEST_PRIME_LOCK.unlock();
    }
    
}

void check_if_prime(const unsigned long long &num_to_factor) {
    
    
        Factor factor(num_to_factor);
        if (factor.check_is_prime()) {
            std::cout << num_to_factor << " is a Prime \n";
            LARGEST_PRIME_LOCK.lock();
            if (num_to_factor > largest_prime)
                largest_prime = num_to_factor;
            LARGEST_PRIME_LOCK.unlock();
        }
        else std::cout << num_to_factor <<" is not a Prime\n";
    

}

//pre: base_value and max_threads must be delcared and defined with positive integers.
//post:
void run_continuous_thread_calculations(const unsigned long long &base_value, const unsigned int &max_threads) {
    bool thread_spawn_running = false;
    std::string command  = "";
    std::future<void> future;
    
    
    future = std::async(thread_spawner, base_value, max_threads);
    thread_spawn_running = true;
    while(!close_program) {
        
        std::cout << "Enter Command: ";
        std::cin >> command;
        std::cout << "\n";
        
        
        if (command == "q" || command == "Q") {
            close_program = true;
        }
        else if (command == "L" || command == "l") {
            LARGEST_PRIME_LOCK.lock();
            if (largest_prime != 0)
                std::cout << "the Largest prime is: " << largest_prime << std::endl;
            else {
                std::cout << "no prime found yet \n";
            }
            LARGEST_PRIME_LOCK.unlock();
        }
        
        
        else if (command == "m" || command == "M") {
            std::cout << "l - prints largest current prime\n q - quits program\n";
        }
        
        
        
        
        else
            std::cout << "Invalid command 'm' for menu \n";
        
    }
    
    std::cout << "closing program please wait for threads to stop \n";
    std::cout << "...\n";
    
    if (thread_spawn_running) {
        while (future.wait_for(std::chrono::seconds(0)) == std::future_status::timeout) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

int main(int argc, char ** argv) {
    int c,
        max_threads = 0;
    unsigned long long base_value = 0;
    char * end;
    bool l_flag = false,
         c_flag = false,
         b_flag = false;
    

    if (MAX_THREADS == 0) {
        std::cout<<"System Max Threads 0. \n Program Exiting\n";
        return 0;
    }
    
    while ((c = getopt(argc, argv, "lc:b:f:ap:c:")) != -1) {
        switch (c) {
            case 'f' :
                unsigned long long number_to_factor;
                
                if (*optarg == '-')
                    throw std::invalid_argument("must pass in positive integer");
                
                try {
                    number_to_factor = std::strtoull(optarg, &end, 10);
                }
                catch (std::exception const & e) {
                    std::cout << "number to factor must be a positive integer \nerror: " << e.what() << std::endl <<"program terminating \n";
                    abort();
                }
                
                factor_number(number_to_factor);
               
            case 'l' :
                l_flag = true;
                break;
                
            case 'c' :
                c_flag = true;
                
                if (*optarg == '-')
                    throw std::invalid_argument("xmust pass in positive integer");
                
                if (l_flag) {
                    

                    try {
                        max_threads = atoi(optarg);
                    }
                    catch (std::exception const & e) {
                        std::cout << "thread limit must be a positive integer \nerror: " << e.what() << std::endl <<"program terminating \n";
                        abort();
                    }
                    if (max_threads > MAX_THREADS)
                        max_threads = MAX_THREADS;
                }
                else {
                    unsigned long long num_to_check;
                   
                    try {
                        num_to_check = strtoull(optarg, &end, 10);
                    }
                    catch (std::exception const & e) {
                        std::cout << "argument must be a positive integer \nerror: " << e.what() << std::endl <<"program terminating \n";
                        abort();
                    }
                    check_if_prime(num_to_check);
                }
                
                break;
                
            case 'b':
                b_flag = true;
                
                try {
                    base_value = std::strtoll(optarg, &end, 10);
                }
                catch (std::exception const & e) {
                    std::cout << "base number input must be a positive integer \nerror: " << e.what() << std::endl <<"program terminating \n";
                    abort();
                }
                break;
                
            default:
                abort();
        }
    
    }
    
    
    if (b_flag || (c_flag && l_flag))  {
        if (!b_flag) {
            std:: cout << "Enter Base Number 'must be positive integer'\n";
            std::cin  >> base_value;
        }
        if (!(l_flag && c_flag))
            max_threads = MAX_THREADS;
        run_continuous_thread_calculations(base_value, max_threads);
    }
    
    
    
    
    return 0;
}
