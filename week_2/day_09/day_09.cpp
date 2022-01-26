#include<iostream>
#include<vector>
#include<list>
#include<string>
#include<algorithm>
#include<cstdlib>
#include"utils.h"

// forward function declarations
long long play_marbles(const int &players, const long long &marbles);

int main(){

    std::vector<std::string> delimiters = {" players; last marble is worth "," points"};
    std::vector<std::vector<int>> input = input_to_int_2D(read_input_2D("input_09",delimiters));

    const long long marbles = input[0][1];
    const int  players = input[0][0];

    std::cout << "Answer (part 1): " << play_marbles(players,marbles) << std::endl;
    std::cout << "Answer (part 2): " << play_marbles(players,marbles*100) << std::endl;

    return 0;
}

long long play_marbles(const int &players, const long long &marbles){
    
    // start with marble zero
    std::list<long long> circle = {0};

    // iterator to current (start at 0)
    auto it = circle.begin();

    // vector of players points
    std::vector<long long> points(players,0);

    // add marbles to circle
    for (long long i=1; i<=marbles; i++){

        if (i % 23 == 0){
            // add next marbe to points
            points[(i-1)%players] += i;

            // marble 7 anticlockwise removed and added to points
            auto it_to_remove = next_cyclic(it,-7,circle);
            points[(i-1)%players] += *it_to_remove;
            it = circle.erase(it_to_remove);
            if (it==circle.end()){ it=circle.begin(); }
        }
        // insert next marble
        else { it = circle.insert(next_cyclic(it,2,circle),i); }
    }

    // find player with most points
    return *std::max_element(points.begin(), points.end());
}