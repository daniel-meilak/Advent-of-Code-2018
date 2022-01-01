#include<iostream>
#include<vector>
#include<string>
#include<utility>
#include<algorithm>
#include<cstdlib>
#include"../../Utils/utils.h"

// forward function declarations
void neighbours(int &open, int &tree, int &lumber, const int &x, const int &y, const std::vector<std::string> &grid);

int main(){

    // read input into vector of strings.
    std::vector<std::string> input = read_input("input_18", "");

    size_t height = input.size()+2;
    size_t width  = input[0].size()+2;

    // grid of acres with border
    std::vector<std::string> grid(height, std::string(width,'.'));

    // fill grid with input
    for (size_t y=0; y<height-2; y++){
        for (size_t x=0; x<width-2; x++){
            grid[y+1][x+1] = input[y][x];
        }
    }

    // copy of grid
    std::vector<std::string> copy  = grid;
    std::vector<std::string> part1; 

    // from visual inspection of output
    // after 500 minutes, grid repeats with period of 28

    // number of iter (minutes)
    int minutes = 524;

    for (int t=0; t<minutes; t++){
        
        // loop through grid
        for (size_t y=1; y<height-1; y++){
            for (size_t x=1; x<width-1; x++){
                
                // neighbours
                int open   = 0;
                int tree   = 0;
                int lumber = 0;

                neighbours(open, tree, lumber, x, y, grid);

                switch(grid[y][x]){
                    // if open
                    case('.'):
                        // change to tree if 3 or more adjacent
                        if (tree>=3){ copy[y][x] = '|'; }
                        break;
                    // if tree
                    case('|'):
                        // change to lumber if 3 or more adjacent
                        if (lumber>=3){ copy[y][x] = '#'; }
                        break;
                    // if lumber
                    case('#'):
                        // if no lumber or tree adjacent, change to open
                        if (lumber==0 || tree==0){ copy[y][x] = '.'; }
                }
            }
        }

        // update grid
        grid = copy;

        // save state at 10 mins
        if (t==9){ part1 = grid; }
    }

    // number of lumber x number of trees in grid
    int tot_trees1  = 0, tot_trees2  = 0;
    int tot_lumber1 = 0, tot_lumber2 = 0;
    for (size_t y=1; y<height-1; y++){
        for (size_t x=1; x<width-1; x++){
            if (part1[y][x]=='|'){ tot_trees1++; }
            else if (part1[y][x]=='#'){ tot_lumber1++; }

            if (grid[y][x]=='|'){ tot_trees2++; }
            else if (grid[y][x]=='#'){ tot_lumber2++; }
        }
    }

    std::cout << "Answer (part 1): " << tot_lumber1*tot_trees1 << std::endl;
    std::cout << "Answer (part 2): " << tot_lumber2*tot_trees2 << std::endl;

    return 0;
}

// return number of adjecent open/tree/lumber areas 
void neighbours(int &open, int &tree, int &lumber, const int &x, const int &y, const std::vector<std::string> &grid){

    std::vector<std::pair<int,int>> neighbours = { {1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1} };

    for (const auto &pos : neighbours){
        switch (grid[y+pos.second][x+pos.first]){
            case('.'):
                open++;
                break;
            case('|'):
                tree++;
                break;
            case('#'):
                lumber++;
                break;
        }
    }
}