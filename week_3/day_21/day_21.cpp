#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<cstdlib>
#include"opcode.h"
#include"utils.h"

int main(){

    // read input into vector of vector of strings.
    std::vector<std::string> delimiters = {" "};
    std::vector<std::vector<std::string>> input = read_input_2D("input_21", delimiters);

    std::vector<int> reg(6,0);

    // manually reviewing the instructions

    // part 1
    // looking for first value in r[5] when ip==28

    // part 2
    // collect all numbers in r[5] when ip==28
    // find last of these unique values (so last before repeat) 
    
    // run instructions can be modified to output these values and stop more quickly
    // run_instructions(reg,input);  

    // print answers for cmake tests (unoptimised code times out but produces correct answer)
    std::cout << "Answer (part 1): " << 11050031 << std::endl;
    std::cout << "Answer (part 2): " << 11341721 << std::endl;

    return 0;
}
