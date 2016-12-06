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
void thread_spawner (unsigned long start, int max_threads) {
    
    unsigned long threads_interval_begin = start;
    std::future<void> *future;
    std::list<std::future<void>*> threads;
    
    // loops while program close command has not been given.
    

    
    while(!close_program) {
        if (threads.size() < max_threads) {
            try {
                future = new std::future<void>;
                // CREATES THREADS BREAKING PROBLEM INTO SECTIONS OF 3,000,000 EACH
                *future = std::async(findPrimeInInterval, threads_interval_begin, threads_interval_begin + 3000000L);
                threads_interval_begin += 3000000L;
                threads.push_back(future);
            }
            catch(std::system_error &e) {
                std::cout << "\n\nSystem error for creating thread \n\n";
                std::cout << &e;
            }
        }
        else {
            
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            
            for (std::list<std::future<void>*>::iterator itr = threads.begin(); itr != threads.end(); itr++) {
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
        for (std::list<std::future<void>*>::iterator itr = threads.begin(); itr != threads.end(); itr++) {
            std::future<void> *thread = *itr;
            auto status = thread->wait_for(std::chrono::seconds(0));
            if (status == std::future_status::ready) {
                delete *itr;
                itr = threads.erase(itr);
            }
        }

    
}


int main(int argc, const char * argv[]) {
    
    unsigned long i = std::atol(argv[1]);
    bool thread_spawn_running = false;
    std::future<void> future;
    
    char *input_integer;
    
    if (argc < 2) {
        std:: cout << "Enter Start Number \n";
        std::cin  >> input_integer;
        i = std::atol(input_integer);
    }
    
    
    
    if (i % 2 == 0)
        ++i;
    
    while(true) {
        std::string command  = "";
        std::cout << "enter command: \n";
        std::cin >> command;
        if (command == "q" || command == "Q") {
            close_program = true;
            break;
        }
        else if (command == "lp" || command == "LP") {
            LARGEST_PRIME_LOCK.lock();
            if (largest_prime != 0)
                std::cout << "the Largest prime is: " << largest_prime << std::endl;
            else {
                std::cout << "no prime found yet \n";
            }
            LARGEST_PRIME_LOCK.unlock();
        }
        else if (command == "fa" || command == "FA") {
            std::string max_threads;
            int max_threads_int;
            
            if (thread_spawn_running) {
                std::cout << "Already calculating all primes";
                continue;
            }
            
            std::cout << "Enter max number of threads \n";
            std::cin >> max_threads;
            try {
                max_threads_int = std::stoi(max_threads);
            } catch (std::exception const & e) {
                std::cout << "threads input must be an integer \nerror: " << e.what() << std::endl;
            }
            
            future = std::async(thread_spawner, i, max_threads_int);
            thread_spawn_running = true;
        }
        else if (command == "m" || command == "M") {
            std::cout << "lp - prints largest prime\nfa start calculating all prime starting from a base value\nq - quits program\n";
        }
        else
            std::cout << "Invalid command try again \n";
        
    }
    
    std::cout << "closing program please wait for threads to stop \n";
    std::cout << "...\n";
    
    if (thread_spawn_running)
        while (future.wait_for(std::chrono::seconds(0)) == std::future_status::timeout) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    
    
    return 0;
}
