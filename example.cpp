/*
complie : clang++  -std=c++11 -O3 example.cpp
output : {
Solution found : true
Path Weight : 41.6272
Path : 
28 22 15 10 16 11 10 5 11 17 22 16 23 29 28 22 15 10 9 14 7 1 0 6 13 12 18 25 20 15 21 14 7 1 2 3 
}
*/
#include <iostream>
#include "pathfinder.h"

//6*5 row-major input board (initial)
std::vector<int> in; 
//6*5 row-major output board (desired)
std::vector<int> out; 
int main() {
	//index to rune type map (index,color) : (0,blue) , (1,red) , (2,green) , (3,yellow) , (4,purple) , (5,pink)
	in = {
	1 , 2 , 0 , 0 , 3 , 5 , 
	0 , 5 , 1 , 1 , 4 , 1 , 
	5 , 0 , 1 , 0 , 3 , 0 ,
	0 , 5 , 5 , 2 , 4 , 2 ,
	3 , 2 , 1 , 1 , 3 , 0 
}; 
	out = {
	0 , 0 , 0 , 3 , 3 , 3 , 
	0 , 1 , 1 , 1 , 1 , 0 , 
	0 , 5 , 2 , 2 , 2 , 0 ,
	2 , 5 , 5 , 5 , 4 , 0 ,
	3 , 5 , 1 , 1 , 1 , 4 
}; 

float pathWeightBound = 65.0f; //arbitrary 

TreeNode sol; //stores solution 

bool found = pf56_single(in,out,28,pathWeightBound,sol); //we start at position 28 

std::cout << "Solution found : " << ( (found == 1) ? "true" : "false" ) << std::endl;
std::cout << "Path Weight : " << sol.pastErr << std::endl;
std::cout << "Path : " << std::endl;
for (auto& v : sol.leadv) {
    std::cout << v << ' ';
}
std::cout << std::endl;
//Then we can execute the solution manually or automatically (see video)
}
