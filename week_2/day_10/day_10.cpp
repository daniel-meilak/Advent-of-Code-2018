#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<cstdlib>
#include<climits>
#include"../../Utils/utils.h"

int main(){

    // read input into vector of vector of strings.
    std::vector<std::string> delimiters = {"position=<",", ","> velocity=<",">"," "};
    std::vector<std::vector<int>> input = input_to_int_2D(read_input_2D("input_10", delimiters));

    // save time of closest points
    int time;

    // copy of input
    std::vector<std::vector<int>> copy = input;

    // smallest bounding box area
    unsigned int smallest_area = INT_MAX; 
    int s_xmin, s_xmax, s_ymin, s_ymax;

    int iter = 20000;
    size_t size = input.size();

    // move particles and calculate bounding box size
    for (int i=1; i<iter; i++){

        // move positions
        for (size_t j=0; j<size; j++){
            input[j][0] += input[j][2];
            input[j][1] += input[j][3];
        }

        // find bounding box
        int xmin = min_col(input,0);
        int ymin = min_col(input,1);
        int xmax = max_col(input,0);
        int ymax = max_col(input,1);
        
        // area
        unsigned int area = (xmax-xmin)*(ymax-ymin);
        if (area < smallest_area){
            smallest_area = area;
            s_xmin = xmin;
            s_xmax = xmax;
            s_ymin = ymin;
            s_ymax = ymax;
            time = i;
        }
    }

    // recover closest distance time using copy
    for (size_t i=0; i<size; i++){
        copy[i][0] += copy[i][2]*time;
        copy[i][1] += copy[i][3]*time;
    }

    std::vector<std::string> grid(1+s_ymax-s_ymin,std::string(1+s_xmax-s_xmin,' '));

    // fill grid with positions
    for (const auto &line : copy){
        grid[line[1]-s_ymin][line[0]-s_xmin] = '#';
    }

    std::cout << "Answer (part 1): " << std::endl;

    // print grid
    display(grid);
    
    std::cout << "Answer (part 2): " << time << std::endl;

    return 0;
}
