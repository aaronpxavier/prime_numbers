# prime_numbers

Program calculates prime numbers, factors any positive integer, and can check if a passed in argument (any positive integer) is a prime. 
The Purpose of this project is to experiment with multithreading in C++; 

#instructions on compiling

g++ main.cpp  prime.cpp -o prime_calc -pthread -std=c++11 
chmod +x pime_calc

# usage 

to start calculating all prime numbers that are greater than 100 
./prime_calc -b 100 

to start calculating all prime numbers greater than 33 but limits number of prime calculation threads to 4. Otherwise program utilizes all available cores. 
./prime_calc -lc 4 -b 100 

to get all postive factors for a given positive integer. Example factors 88 
./prime_calc -f 88 

to check if given positive integer is a prime 
./prime_calc -c 3 

#license 

MIT License

Copyright (c) 2016 amavumkal

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. 






