#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<climits>
#include<limits>
#include<cstdlib>
#include<utility>
#include"../../Utils/utils.h"

// function to compare nth element in vec of vec
struct sort_by_sr{
    int col;

    sort_by_sr(int col_): col(col_){};

    bool operator()(const std::vector<long long> &lhs, const std::vector<long long> &rhs){
        if (lhs[col]==rhs[col]){ return manhattan_3D(lhs[0],lhs[1],lhs[2]) < manhattan_3D(rhs[0],rhs[1],rhs[2]); }
        else {return lhs[col] < rhs[col]; }
    }
};


bool sort_by_prox(const std::pair<std::vector<double>,int> &lhs, const std::pair<std::vector<double>,int> &rhs){
    if (lhs.second==rhs.second){ return lhs.first[3]<rhs.first[3]; }
    else {return lhs.second > rhs.second; }
}

// forward function declarations
int part1(const std::vector<std::vector<long long>> &input);
int part2(const std::vector<std::vector<long long>> &input);
std::vector<std::vector<double>> to_double(const std::vector<std::vector<long long>> &input);

int main(){

    // read input into vector of vector of strings.
    std::vector<std::string> delimiters = {",","pos=<"," ",">","r="};
    std::vector<std::vector<long long>> input = input_to_llint_2D(read_input_2D("input", delimiters));

    

    std::cout << "Answer (part 1): " << part1(input) << std::endl;
    std::cout << "Answer (part 2): " << part2(input) << std::endl;

    return 0;
}

int part1(const std::vector<std::vector<long long>> &input){
    
    // find nanobot with highest signal radius
    auto it = std::max_element(input.begin(), input.end(), sort_by_sr(3));

    // highest signal radius value
    long long highest_radius = (*it)[3];

    // find number of nanobots in range of highest signal radius
    int count = 0;
    for (const std::vector<long long> &nano : input){

        if (manhattan_3D(nano[0],nano[1],nano[2],(*it)[0],(*it)[1],(*it)[2]) <= highest_radius){ count++; }       
    }

    return count;
}

int part2(const std::vector<std::vector<long long>> &input){

    // vector for min max in x,y,z directions
    std::vector<double> min(3,std::numeric_limits<double>::max()), max(3,std::numeric_limits<double>::min());
    
    // start by reducing system dimensions by 1m
    int size_reduce = std::pow(2,28);

    // find system borders
    int dim = 3;
    for (auto line : input){
        for (int i=0; i<dim; i++){
            min[i] = std::min(min[i],(double)line[i]);
            max[i] = std::max(max[i],(double)line[i]);
        }
    }

    // reduce borders by size_reduce
    for (int i=0; i<dim; i++){
        min[i] /= size_reduce;
        max[i] /= size_reduce;
    }

    // best pos for each iter
    std::vector<double> best_pos(3);

    // stop when we find coordinate at full size
    while (size_reduce!=0){

        // copy of input and distances
        std::vector<std::vector<double>> copy = to_double(input);

        // reduce system by size_reduce
        for (auto &line: copy){

            for (auto &coord : line){
                coord /= size_reduce;
            }
        }

        // check each pos in reduced box
        std::vector<std::pair<std::vector<double>,int>> all_pos;
        std::vector<double> inc(3);
        for (int i=0; i<dim; i++){
            inc[i] = (max[0]-min[0])/5.0;
        }

        // adjust min and max to help with edge cases
        for (int i=0; i<dim; i++){
            min[i] -= inc[i];
            max[i] += inc[i];
        }

        for (double x=min[0]; x<=max[0]; x+=inc[0] ){
            for (double y=min[1]; y<=max[1]; y+=inc[1] ){
                for (double z=min[2]; z<=max[2]; z+=inc[2] ){
                    
                    int count = 0;
                    // count number of nanobots int range
                    for (const auto &nano : copy){
                        if (manhattan_3D(x,y,z,nano[0],nano[1],nano[2])<=nano[3]){
                            count+=1.0;
                        }
                        else {
                            std::cout << "NOPE" << std::endl;
                            std::cout << manhattan_3D(x,y,z,nano[0],nano[1],nano[2]) << std::endl;
                            std::cout << nano[3] << std::endl;
                            std::cout << std::endl;
                        }
                    }

                    if (count > 1){
                        std::vector<double> pos = {x,y,z,manhattan_3D(x,y,z)};

                        all_pos.push_back({pos,count});
                    }
                }
            }
        }

        std::cout << 57429458/(double)size_reduce << "  " << 47789543/(double)size_reduce << "  " << 59741497/(double)size_reduce << std::endl;

        // sort all pos by biggest proximity
        std::sort(all_pos.begin(), all_pos.end(), sort_by_prox);
        best_pos = all_pos[0].first;

        // resize borders to include all highest proximity points
        int highest = all_pos[0].second;
        // reset min and max
        std::fill(min.begin(), min.end(), std::numeric_limits<double>::max());
        std::fill(max.begin(), max.end(), std::numeric_limits<double>::min());
        for (const auto &pair : all_pos){
            if (pair.second==highest){
                for (int i=0; i<3; i++){
                    min[i] = std::min(min[i], 2*(pair.first[i]-1));
                    max[i] = std::max(max[i], 2*(pair.first[i]+1)); 
                }
            }
        }
        
        // halve size reduce
        size_reduce /= 2;
    }

    // return manhattan distance of best pos
    return manhattan_3D(best_pos[0], best_pos[1], best_pos[2]);
}

// convert 2D vector of int to 2D vector of double
std::vector<std::vector<double>> to_double(const std::vector<std::vector<long long>> &input){

    int height = input.size();
    int width  = input[0].size(); 
    
    std::vector<std::vector<double>> output(height, std::vector<double>(width));


    for (int y=0; y<height; y++){
        for (int x=0; x<width; x++){
            output[y][x] = (double)input[y][x];
        }
    }

    return output;
}