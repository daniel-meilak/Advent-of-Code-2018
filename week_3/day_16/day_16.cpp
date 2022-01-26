#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<cstdlib>
#include<map>
#include"utils.h"
#include"opcode.h"

int main(){

    // read input into vector of vector of strings.
    std::vector<std::string> delimiters = {"Before:","[","]"," ","After:",","};
    std::vector<std::vector<int>> input = input_to_int_2D(read_input_2D("input_16", delimiters));
    std::vector<std::vector<int>> samples, opcode;

    // split into samples and opcode
    int spaces  = 0;
    size_t line = 0;
    while (spaces!=3){
        if (input[line].empty()){ spaces++; }
        else                    { spaces=0; }

        samples.push_back(input[line++]);
    }

    while (line!=input.size()){ opcode.push_back(input[line++]); }

    // registers and arguments vector
    std::vector<int> reg, arg; 

    // map of unknown function id to functions indexes
    std::map<int,std::vector<int>> links;

    // number of samples with 3 or more possible opcodes
    int count = 0;

    // work through samples
    size_t size = samples.size()-4;
    for (size_t i=0; i<size; i+=4){

        // set args from samples
        arg = samples[i+1];

        // number of functions that work on current sample
        int valid_func = 0;

        // try each function
        size_t num_func = functions.size();
        for (size_t j=0; j<num_func; j++){

            // set reg from samples
            reg = samples[i];
            
            functions[j](arg[1], arg[2], arg[3], reg);

            // if register is now the same as after, add to map
            if (reg==samples[i+2]){

                // unknown function id
                int unknown_id = arg[0];

                // vector of possible functions for id
                std::vector<int> &possible = links[unknown_id];

                // if function j has not already been verified, add to possible list
                if (std::find(possible.begin(), possible.end(), j)==possible.end()){ possible.push_back(j); }

                // increace valid func count
                valid_func++;
            }
        }

        // if sample had 3 or move valid functions, inc count
        if (valid_func >= 3){ count++; }
    }

    std::cout << "Answer (part 1): " << count << std::endl;

    // worked out which function is which by hand
    std::map<int,int> resolve;

    resolve[0]  = 7;
    resolve[1]  = 6;
    resolve[2]  = 9;
    resolve[3]  = 2;
    resolve[4]  = 8;
    resolve[5]  = 0; 
    resolve[6]  = 10;
    resolve[7]  = 13;
    resolve[8]  = 11;
    resolve[9]  = 5;
    resolve[10] = 3;
    resolve[11] = 12;
    resolve[12] = 4;
    resolve[13] = 14;
    resolve[14] = 1;
    resolve[15] = 15;

    // reset register
    reg = {0,0,0,0};

    // work through opcode
    for (const std::vector<int> &line : opcode){

        // line[0] is function id (unresolved), line[1-3] are function arguments
        functions[resolve[line[0]]](line[1], line[2], line[3], reg); 
    }

    std::cout << "Answer (part 2): " << reg[0] << std::endl;

    return 0;
}

