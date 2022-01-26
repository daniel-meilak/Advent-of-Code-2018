#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<cstdlib>
#include"utils.h"

const int input = std::stoi(read_line("input_11"));

// forward function declarations
int power(const int &x, const int &y);
int nth_digit(const int &x,const int &n);
std::vector<int> region(const int &box_size);

int main(){

    // find best size box
    std::vector<int> part2 = {0,0,0};
    int best_size = 0;

    for (int i=1; i<=20; i++){
        std::vector<int> config = region(i);
        if (config[2]>part2[2]){
            part2 = config;
            best_size = i;
        }
    }

    // part 1
    std::vector<int> part1 = region(3);

    std::cout << "Answer (part 1): " << part1[0] << "," << part1[1] << std::endl;
    std::cout << "Answer (part 2): " << part2[0] << "," << part2[1] << "," << best_size << std::endl;

    return 0;
}

std::vector<int> region(const int &box_size){
    
    // store largest 3x3 power
    int max_power = 0;
    int x_max = 0;
    int y_max = 0;

    // grid from 1-300
    const int size = 300-(box_size-1);
    for (int x=1; x<=size; x++){
        for (int y=1; y<=size; y++){

            // calculate total power for 3x3 
            int total = 0;
            for (int i=x; i<x+box_size; i++){
                for (int j=y; j<y+box_size; j++){
                    total += power(i,j);
                }
            }

            // check if total is highest
            if (total>max_power){
                max_power = total;
                x_max = x;
                y_max = y;
            }
        }
    }

    return {x_max,y_max,max_power};
}

// calculate power level of coordinate
int power(const int &x, const int &y){

    int rack_id = x+10;
    int power_level = (rack_id*y)+input;
    
    power_level *= rack_id;

    return nth_digit(power_level,2)-5;
}

// nth digit of int
int nth_digit(const int &x,const int &n){
    return x/(int)std::pow(10,n) % 10;
}