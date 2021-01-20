#include<iostream>
#include<vector>
#include<list>
#include<string>
#include<algorithm>
#include<cstdlib>
#include"../../Utils/utils.h"

// forward function declarations
long play_marbles(const int &players, const long &marbles);

int main(){

    const long marbles = 72019;
    const int players = 458;

    std::cout << "Answer (part 1): " << play_marbles(players,marbles) << std::endl;
    std::cout << "Answer (part 2): " << play_marbles(players,marbles*100) << std::endl;

    return 0;
}

long play_marbles(const int &players, const long &marbles){
    
    // start with marble zero
    std::list<long> circle = {0};

    // iterator to current (start at 0)
    auto it = circle.begin();

    // vector of players points
    std::vector<long> points(players,0);

    // add marbles to circle
    for (long i=1; i<=marbles; i++){

        if (i % 23 == 0){
            // add next marbe to points
            points[(i-1)%players] += i;

            // marble 7 anticlockwise removed and added to points
            auto it_to_remove = next_cyclic(it,-7,circle);
            points[(i-1)%players] += *it_to_remove;
            it = circle.erase(it_to_remove);
            if (it==circle.end()){ it=circle.begin(); }
        }
        else {
            // insert next marble
            it = circle.insert(next_cyclic(it,2,circle),i);
        }
    }

    // find player with most points
    return *std::max_element(points.begin(), points.end());
}