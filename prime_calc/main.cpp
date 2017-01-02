//
//  main.cpp
//  prime_calc
//
//  Created by Arun Mavumkal on 11/29/16.
//  Copyright © 2016 Arun Mavumkal. All rights reserved.
//

#include <iostream>
#include <thread>
#include "prime.hpp"
#include <chrono>
#include <future>
#include <list>
#include <mutex>


const unsigned int MAX_THREADS = std::thread::hardware_concurrency();
unsigned long largest_prime;

std::mutex LARGEST_PRIME_LOCK;

std::mutex CLOSE_PROGRAM_LOCK;

bool close_program = false;


//pre: no arguments taken.
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
//post. 
void find_prime(unsigned long integer_to_check) {
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
//post:functions ends when i reaches finish.
void findPrimeInInterval(unsigned long start, const unsigned long FINISH) {
    
    if (start % 2 == 0)
        ++ start;
    
    for (unsigned long i = start; i < FINISH; i+=2) {
        if (close_program) break;
        find_prime(i);
    }
}

//pre i and max_threads must be declared and defined with positive integers.
//post: fn stops when close program is true and all open prime calculation threads are closed.
void thread_spawner (unsigned long start) {
    
    unsigned long threads_interval_begin = start;
    std::future<void> *future;
    std::list<std::future<void>*> threads;
    
    // loops while program close command has not been given.
    while(!close_program) {
        if (threads.size() < MAX_THREADS) {
            try {
                future = new std::future<void>;
                // CREATES THREADS BREAKING PROBLEM INTO SECTIONS OF 3,000,000 EACH
                *future = std::async(findPrimeInInterval, threads_interval_begin, threads_interval_begin + 3000000L);
                threads_interval_begin += 3000000L;
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


int main(int argc, const char * argv[]) {
    
    bool thread_spawn_running = false;
    std::future<void> future;
    
    if (MAX_THREADS == 0) {
        std::cout<<"System Max Threads 0. \n Program Exiting\n";
        return 0;
    }
    
    while(true) {
        std::string command  = "";
        
        std::cout << "enter command: \n";
        std::cin >> command;
        
        if (command == "q" || command == "Q") {
            close_program = true;
            break;
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
        
        else if (command == "fa" || command == "FA")
        {
            std::string max_threads, input_integer_str;
            unsigned long input_integer;
            
            if (thread_spawn_running) {
                std::cout << "Already calculating all primes";
                continue;
            }
            
            std:: cout << "Enter Base Number 'must be positive integer'\n";
            std::cin  >> input_integer_str;
            
            try {
                input_integer = std::stoi(input_integer_str);
            }
            catch (std::exception const & e) {
                std::cout << "base number input must be a positive integer \nerror: " << e.what() << std::endl <<"Returning to Menu\n";
                continue;
                
            }
            
            
            std::cout << "Enter max number of threads 'must be positive integer'\n";
            std::cin >> max_threads;
            
           
            
            
            future = std::async(thread_spawner, input_integer);
            thread_spawn_running = true;
        }
        
        else if (command == "m" || command == "M") {
            std::cout << "l - prints largest prime\nfa start calculating all prime starting from a base value\nq - quits program\n" <<
                         "factor - prints all positive factors for given integer\n check - checks if given integer is a prime";
        }
        
        else if (command == "factor" || command == "FACTOR") {
            std::string num_to_factor_str;
            unsigned long num_to_factor;
            std::cout << "Enter Number To Factor:\n";
            std::cin >> num_to_factor_str;
            try {
                num_to_factor = std::stoul(num_to_factor_str);
            }
            catch (std::exception const &e) {
                std::cout << "Number must be valid integer\nerror: " << e.what() << std::endl <<"Returning to Menu\n";
                continue;
            }
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
        
        else if (command == "check" || command == "CHECK") {
            std::string num_to_factor_str;
            unsigned long num_to_factor;
            std::cout << "Enter Number To Factor:\n";
            std::cin >> num_to_factor_str;
            try {
                num_to_factor = std::stoul(num_to_factor_str);
            }
            catch (std::exception const &e) {
                std::cout << "Number must be valid integer\nerror: " << e.what() << std::endl <<"Returning to Menu\n";
                continue;
            }
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
        
        else
            std::cout << "Invalid command 'm' for menu \n";
        
    }
    
    std::cout << "closing program please wait for threads to stop \n";
    std::cout << "...\n";
    
    if (thread_spawn_running)
        while (future.wait_for(std::chrono::seconds(0)) == std::future_status::timeout) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    
    
    return 0;
}
