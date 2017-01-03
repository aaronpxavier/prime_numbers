# prime_numbers

Program calculates prime numbers, factors any positive integer, and can check if a passed in argument (any positive integer) is a prime. 
The Purpose of this project is to experiment with multithreading in C++; 


# usage 

to start calculating all prime numbers that are greater than 100 
./prime_calc -b 100 

to start calculating all prime numbers greater than 33 but limits number of prime calculation threads to 4. Otherwise program utilizes all available cores. 
./prime_calc -lc 4 -b 100 

to get all postive factors for a given positive integer. Example factors 88 
./prime_calc -f 88 

to check if given positive integer is a prime 
./prime_calc -c 3 


