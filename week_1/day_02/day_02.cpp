#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<cstdlib>
#include<unordered_map>
#include"utils.h"

// forward function declarations
void count(const std::string &word, int &two, int &three);
std::string difference(const std::string &word1, const std::string &word2);

int main(){

    // read input into vector of strings.
    std::vector<std::string> input = read_input("input_02");

    // tallies of 2 and 3 letter repeats
    int two = 0;
    int three = 0;

    // check each word for repeat letters
    for (const std::string &line : input){
        count(line,two,three);
    }

    // compare pairs of words to find correct boxes
    std::string common;
    size_t size = input.size();
    for (size_t i=0; i<size; i++){
        for (size_t j=i+1; j<size; j++){

            // find common letters in correct boxes
            common = difference(input[i], input[j]);
            if (common!=""){ break; }
        }
        if (common!=""){ break; }
    }

    std::cout << "Answer (part 1): " << two*three << std::endl;
    std::cout << "Answer (part 2): " << common      << std::endl;

    return 0;
}

void count(const std::string &word, int &two, int &three){

    // map of letter occurences
    std::unordered_map<char, int> letters;

    // tally letter occurences
    for (const char &c : word){
        letters[c]++;
    }

    // checks for 2 and 3 of a letter
    bool two_of=false, three_of=false;

    // check for required tallies
    for (const auto &tally : letters){
        if (tally.second==2){ two_of   = true; }
        if (tally.second==3){ three_of = true; }
    }

    // increment counters
    if (two_of  ){ two++;   }
    if (three_of){ three++; }
}

std::string difference(const std::string &word1, const std::string &word2){

    size_t size = word1.size();

    // output string
    std::string output;

    // number of different letters
    int diff = 0;

    // compare each letter
    for (size_t i=0; i<size; i++){
        if (word1[i]!=word2[i]){ diff++; }
    }

    // if difference is 1 return the common letters
    if (diff==1){
        for (size_t i=0; i<size; i++){
            if (word1[i]==word2[i]){ output.push_back(word1[i]); }
        }
    }
    
    return output;
}