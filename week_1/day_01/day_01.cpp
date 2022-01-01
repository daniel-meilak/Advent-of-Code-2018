#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<cstdlib>
#include<set>
#include"../../Utils/utils.h"

// forward function declarations
int part1(const std::vector<int> &input);
int part2(const std::vector<int> &input);

int main(){

    // read input into vector of strings.
    std::vector<int> input = input_to_int(read_input("input_01", ""));

    std::cout << "Answer (part 1): " << part1(input) << std::endl;
    std::cout << "Answer (part 2): " << part2(input) << std::endl;

    return 0;
}

int part1(const std::vector<int> &input){

    // current frequency
    int freq = 0;

    // work through input
    for (int line : input){
        freq += line;
    }

    return freq;
}

int part2(const std::vector<int> &input){

    // current frequency
    int freq = 0;

    // history of frequencies, start with 0
    std::set<int> history = {0};
    bool repeat = false;

    // work through input
    while(!repeat){
        for (const int &line : input){
            freq += line;

            if (history.contains(freq)){
                repeat = true;
                break;
            }
            else { history.insert(freq); }
        }
    }

    return freq;
}
