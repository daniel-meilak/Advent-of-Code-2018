#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<cstdlib>
#include"../../Utils/utils.h"

int main(){

    // read input into vector of vector of strings.
    std::vector<std::string> delimiters = {" ","#","@",",",":","x"};
    std::vector<std::vector<int>> input = input_to_int_2D(read_input_2D("input_03", delimiters));

    const int grid_size = 1000;

    // grid of 1000x1000
    std::vector<std::vector<int>> grid(grid_size, std::vector<int>(grid_size,0));

    // fill in grid with each rectangle
    for (const auto &line : input){
        
        // position of top corner
        int x = line[1];
        int y = (grid_size-line[2])-1;

        int y_min = y-line[4];
        int x_max = x+line[3];
        for (int i=y; i>y_min; i--){
            for (int j=x; j<x_max; j++){
                grid[i][j]++;
            }
        }
    }

    // tally all overlapped grid positions
    int overlapped = 0;
    for (int i=0; i<grid_size; i++){
        for (int j=0; j<grid_size; j++){
            if (grid[i][j]>1){ overlapped++; }
        }
    }


    // flag for overlapping rectangle
    bool overlap = false;

    // non-overlapping rectangle id
    int id{0};

    // loop through rectangles again to find one which doenst overlap
    for (const auto &line : input){

        // position of top corner
        int x = line[1];
        int y = (grid_size-line[2])-1;

        int y_min = y-line[4];
        int x_max = x+line[3];
        for (int i=y; i>y_min; i--){
            for (int j=x; j<x_max; j++){
                if (grid[i][j]>1){ overlap = true; }
            }
        }

        // if overlap remains false, found correct id
        if (!overlap){
            id = line[0];
            break;
        }

        // reset overlap
        overlap = false;
    }

    std::cout << "Answer (part 1): " << overlapped << std::endl;
    std::cout << "Answer (part 2): " << id         << std::endl;

    return 0;
}
