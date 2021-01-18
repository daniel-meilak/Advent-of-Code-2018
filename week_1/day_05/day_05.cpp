#include<iostream>
#include<vector>
#include<list>
#include<string>
#include<algorithm>
#include<cstdlib>
#include<cctype>
#include"../../Utils/utils.h"

// forward function declaration
int reduce(std::list<char> reaction);

int main(){

    // read input into string.
    std::string input = read_line("input", {});

    // convert input to list of char
    std::list<char> reaction(input.begin(), input.end());

    // shortest length of reaction
    int shortest = 1000000;

    for (int i='a'; i<='z'; i++){

        // copy of reaction
        std::list<char> reaction2 = reaction;

        // remove all of letter (upper and lower case)
        reaction2.remove(i);
        reaction2.remove(std::toupper(i));

        shortest = std::min(shortest, reduce(reaction2));
    }

    std::cout << "Answer (part 1): " << reduce(reaction) << std::endl;
    std::cout << "Answer (part 2): " << shortest         << std::endl;

    return 0;
}

// reduces reaction and returns end size
int reduce(std::list<char> reaction){

    // loop through pairs of letters
    for (auto it=reaction.begin(); it!=reaction.end();){
        
        // if value at it is uppercase
        if (std::isupper(*it)){
            // check if next value is lowercase version
            if (std::tolower(*it)!=*std::next(it)){
                it++;
                continue;
            }
        }
        // value at it is lowercase
        else {
            // check if next value is uppercase version
            if (std::toupper(*it)!=*std::next(it)){
                it++;
                continue;
            }
        }

        // remove pair
        it = reaction.erase(it,std::next(it,2));
        if (it!=reaction.begin()){ it--; }
    }

    return reaction.size();
}