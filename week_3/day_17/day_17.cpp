#include<iostream>
#include<vector>
#include<list>
#include<string>
#include<algorithm>
#include<climits>
#include<cstdlib>
#include<utility>
#include<iomanip>
#include<thread>
#include<chrono>
#include"utils.h"

using namespace std::chrono_literals;

int main(){

    // read input into vector of vector of strings.
    std::vector<std::string> delimiters = {"=",","," ",".."};
    std::vector<std::vector<std::string>> input = read_input_2D("input_17", delimiters);

    // first assess boundaries
    int ymin_clay = INT_MAX;
    int xmin = INT_MAX;
    int ymin = 0;
    int xmax = 0;
    int ymax = 0;

    for (const std::vector<std::string> &line : input){
        if (line[0]=="x"){
            xmin = std::min(xmin,std::stoi(line[1]));
            xmax = std::max(xmax,std::stoi(line[1]));
            ymax = std::max(ymax,std::stoi(line[4]));
            ymin_clay = std::min(ymin_clay,std::stoi(line[3]));
        }
        else {
            xmin = std::min(xmin,std::stoi(line[3]));
            xmax = std::max(xmax,std::stoi(line[4]));
            ymax = std::max(ymax,std::stoi(line[1]));
            ymin_clay = std::min(ymin_clay,std::stoi(line[1]));
        }
    }

    // find width and hight and inclide horizontal padding
    int padding = 2;
    int width  = padding+(xmax-xmin)+1;
    int height = (ymax-ymin)+1;

    // adjust xmin by half padding
    xmin -= padding/2;

    // grid map
    std::vector<std::vector<char>> grid(height, std::vector<char>(width,'.'));

    // water at x=500, y=0
    grid[0][500-xmin] = '+';

    // construct clay deposits from input
    for (const std::vector<std::string> &line : input){
        if (line[0]=="x"){
            int x     = std::stoi(line[1]);
            int y_min = std::stoi(line[3]);
            int y_max = std::stoi(line[4]);

            for (int i=y_min; i<=y_max; i++){
                grid[i][x-xmin] = '#';
            }
        }
        else {
            int y     = std::stoi(line[1]);
            int x_min = std::stoi(line[3]);
            int x_max = std::stoi(line[4]);

            for (int i=x_min; i<=x_max; i++){
                grid[y][i-xmin] = '#';
            }
        }   
    }

    // list of water sources
    std::list<std::pair<int,int>> sources = {{500-xmin,0}};

    while (!sources.empty()){

        // current position and 
        int x = sources.front().first;
        int y = sources.front().second;

        // bool to keep track of finished source
        bool found_exit = false;

        while (y < ymax && !found_exit){

            // free to move down
            if (grid[y+1][x] == '.'){

                y++;
                grid[y][x] = '|';                
            }
            // reached another source of flowing water
            // no need to continue
            else if (grid[y+1][x] == '|'){

                found_exit = true;               
            }
            // hit still water or clay
            else if (grid[y+1][x] == '~' || grid[y+1][x] == '#'){

                // first explore left
                int left = x-1;
                while ( (grid[y+1][left]=='~' || grid[y+1][left]=='#') && grid[y][left]!='#'){
                    left--;
                }
                if (grid[y][left]!='#'){
                    found_exit = true;
                    std::pair<int,int> new_source = {left,y};
                    if (std::find(sources.begin(), sources.end(), new_source)==sources.end()){
                        sources.push_back(new_source);
                    }
                }
                else {left++;}
                

                // next explore right
                int right = x+1;
                while ( (grid[y+1][right]=='~' || grid[y+1][right]=='#') && grid[y][right]!='#'){
                    right++;
                }
                if (grid[y][right]!='#'){
                    found_exit = true;
                    std::pair<int,int> new_source = {right,y};
                    if (std::find(sources.begin(), sources.end(), new_source)==sources.end()){
                        sources.push_back(new_source);
                    }
                }
                else {right--;}

                // if overflow fill with '|' else fill with still water '~'
                char fill;
                if (found_exit){ fill='|'; }
                else { fill='~'; }
                for (int w=left; w<=right; w++){
                    grid[y][w] = fill;
                }

                // move up
                y--;              
            }
        }

        // remove source
        sources.pop_front();
    }

    // count water tiles
    int part1 = 0;
    int part2 = 0;
    for (size_t i=ymin_clay; i<grid.size(); i++){
        for (size_t j=0; j<grid[0].size(); j++){
            
            if (grid[i][j]=='|'){ part1++; }
            else if (grid[i][j]=='~'){
                part1++;
                part2++;
            }
        }
    }

    std::cout << "Answer (part 1): " << part1 << std::endl;
    std::cout << "Answer (part 2): " << part2 << std::endl;

    return 0;
}
