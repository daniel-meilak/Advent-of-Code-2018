#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<unordered_map>
#include<utility>
#include<cstdlib>
#include<ranges> // C++20
#include"../../Utils/utils.h"

int main(){

    // read input into vector of vector of strings.
    std::vector<std::string> delimiters = {","," "};
    std::vector<std::vector<int>> input = input_to_int_2D(read_input_2D("input", delimiters));

    // work through input to find xmin,xmax,ymin,ymax
    int xmin = 1000;
    int xmax = 0;
    int ymin = 1000;
    int ymax = 0;

    for (auto &line : input){
        xmin = std::min(xmin,line[0]);
        xmax = std::max(xmax,line[0]);
        ymin = std::min(ymin,line[1]);
        ymax = std::max(ymax,line[1]);
    }

    size_t size = input.size();

    // vector of num nearest neighbours to each square
    std::vector<int> num_nearest(size);

    // number of squares within 10000 summed distance to points
    int region = 0;

    // find nearest squares within bounds
    for (int x=xmin; x<=xmax; x++){
        for (int y=ymin; y<=ymax; y++){

            // vector of distances to each point
            std::vector<int> dist(size);

            // find distance from xy to each point in input and sum 
            int sum = 0;
            for (size_t i=0; i<size; i++){
                dist[i] = manhattan(input[i],{x,y});
                sum += dist[i];
            }

            // if i is less than 10000, increment region
            if (sum<10000){ region++; }

            // if current square is in input, continue
            if (std::ranges::find(input, std::vector<int>(x,y))!=input.end()){ continue; }

            // find min distance
            auto it_min = std::ranges::min_element(dist);

            // check if min distance is repeated, if yes ignore
            if (std::ranges::count(dist, *it_min) > 1){ continue; }

            // add to tally of nearest neighbours
            num_nearest[std::distance(dist.begin(),it_min)]++;
        }
    }

    // reduce num_nearest for positions on borders to zero
    for (size_t i=0; i<size; i++){
        if ( input[i][0]==xmin || input[i][0]==xmax || input[i][1]==ymin || input[i][0]==ymax ){ num_nearest[i]=0; }
    }

    // find largest area
    int largest = *std::max_element(num_nearest.begin(), num_nearest.end());

    std::cout << "Answer (part 1): " << largest << std::endl;
    std::cout << "Answer (part 2): " << region  << std::endl;

    return 0;
}
