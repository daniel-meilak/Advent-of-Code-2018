#include<iostream>
#include<vector>
#include<list>
#include<string>
#include<algorithm>
#include<cstdlib>
#include"utils.h"

// forward function declarations
std::string part1(const std::string &input);
int part2(const std::string &input);

int main(){

    std::string input = read_line("input_14");

    std::cout << "Answer (part 1): " << part1(input) << std::endl;
    std::cout << "Answer (part 2): " << part2(input) << std::endl;

    return 0;
}

std::string part1(const std::string &input){

    std::list<char> scores = {'3','7'};

    // iterators to the first score for each elf
    auto elf1 = scores.begin();
    auto elf2 = std::next(scores.begin());
    
    // create recipies
    while (scores.size()<std::stoul(input)+10){

        // add last scores of each elf
        int next_score = (*elf1 -'0') + (*elf2 -'0');

        // add new recipie to scores
        std::string add_string = std::to_string(next_score);
        for (const char &c : add_string){
            scores.push_back(c);
        }

        // elves choose new recipie
        advance_cyclic(elf1,1+(*elf1-'0'),scores);
        advance_cyclic(elf2,1+(*elf2-'0'),scores);
    }

    // get last 10 recipie scores
    std::string last_ten;
    auto it = --scores.end();
    for (int i=0; i<10; i++){
        last_ten.push_back(*it);
        std::advance(it,-1);
    }

    // reverse last ten
    std::reverse(last_ten.begin(), last_ten.end());

    return last_ten;
}

int part2(const std::string &input){

    std::list<char> scores = {'3','7'};

    // iterators to the first score for each elf
    auto elf1 = scores.begin();
    auto elf2 = std::next(scores.begin());

    // pos of input in scores
    size_t pos = std::string::npos;

    // iterations
    int iter = 0;
    
    // create recipies
    while (pos==std::string::npos){

        // add last scores of each elf
        int next_score = (*elf1 -'0') + (*elf2 -'0');

        // add new recipie to scores
        std::string add_string = std::to_string(next_score);
        for (const char &c : add_string){
            scores.push_back(c);
        }

        // elves choose new recipie
        advance_cyclic(elf1,1+(*elf1-'0'),scores);
        advance_cyclic(elf2,1+(*elf2-'0'),scores);

        // every 1m iter search for input in scores
        if (iter % 1000000 == 0){
            std::string scores_string(scores.begin(), scores.end());
            pos = scores_string.find(input);
        }
        iter++;
    }    
    
    return pos;
}