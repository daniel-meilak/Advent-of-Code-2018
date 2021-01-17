#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<cstdlib>
#include"../../Utils/utils.h"

// forward function declarations
int part1(const std::vector<int> &input);
int part2(const std::vector<int> &input);

int main(){

    // read input into vector of strings.
    std::vector<int> input = input_to_int(read_input("input", ""));

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

    // history of frequencies
    std::vector<int> history;

    // work through input
    const int size = input.size();
    for (int i=0; i<size; i++){
        freq += input[i];

        if (std::find(history.begin(), history.end(), freq)!=history.end()){
            break;
        }

        history.push_back(freq);
        if (i==size-1){ i=-1; }
    }

    return freq;
}
