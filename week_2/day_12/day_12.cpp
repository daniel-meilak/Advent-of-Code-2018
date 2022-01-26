#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<cstdlib>
#include<unordered_map>
#include"utils.h"

// forward function declaration
int plant_count(const int &generations, std::string plants, const std::string &padding, std::unordered_map<std::string,char> &rules);

int main(){

    // read input into vector of vector of strings.
    std::vector<std::string> delimiters = {"=>"," ","initial state:"};
    std::vector<std::vector<std::string>> input = read_input_2D("input_12", delimiters);

    // initial state and copy
    std::string padding(300,'.');
    std::string plants  = padding + input[0][0] + padding;
    
    // build map of rules
    std::unordered_map<std::string, char> rules;

    size_t size = input.size();
    for (size_t i=2; i<size; i++){
        rules[input[i][0]] = input[i][1][0];
    }

    // printing generations up to 200, we see that all future gens increase by 26
    long long part2 = plant_count(200,plants,padding,rules) + (50000000000-200)*26;

    std::cout << "Answer (part 1): " << plant_count(20,plants,padding,rules) << std::endl;
    std::cout << "Answer (part 2): " << part2 << std::endl;

    return 0;
}

int plant_count(const int &generations, std::string plants, const std::string &padding, std::unordered_map<std::string,char> &rules){

    std::string copy = plants;

    // count for plants
    int count = 0;

    // run 20 generations
    size_t size = plants.size()-2;
    for (int i=0; i<generations; i++){

        // check each plant position
        for (size_t j=0; j<size; j++){
            std::string sub = copy.substr(j,5);

            if (rules.count(sub)==1){ plants[j+2] = rules[sub]; }
            else { plants[j+2] = '.'; }
        }

        // update copy
        copy = plants;
    }

    size = plants.size();
    for (size_t i=0; i<size; i++){
        if (plants[i]=='#'){
            count += i-padding.size();
        }
    }

    return count;
}