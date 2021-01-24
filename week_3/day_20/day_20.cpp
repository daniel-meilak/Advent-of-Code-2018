#include<iostream>
#include<vector>
#include<list>
#include<string>
#include<algorithm>
#include<cstdlib>
#include<unordered_map>
#include"../../Utils/utils.h"

// struct for position
struct point{
    int x;
    int y;

    point(int x_, int y_): x(x_), y(y_){}; 
};

// point equality
bool operator==(const point &lhs, const point &rhs){
    return (lhs.x==rhs.x && lhs.y==rhs.y);
}

// hashing funciton for map of positions
namespace std {
    template <> struct hash<point> {
    typedef point argument_type;
    typedef std::size_t result_type;
    std::size_t operator()(const point& pos) const noexcept {
        return std::hash<int>()(pos.x ^ (pos.y << 4));
    }
    };
}

int main(){

    // read input into vector of strings.
    std::string input = read_line("input", {});

    // map of positions
    std::unordered_map<point,int> grid;

    grid[point(0,0)] = 0;
    int dist = 0;
    int x = 0, y = 0;

    std::list<std::vector<int>> stack;

    // work through each step in regex
    const int size = input.size()-1;
    for (int i=1; i<size; i++){
        char &c = input[i];

        // add new path to work through to stack
        if (c=='('){
            stack.push_back({dist,x,y});
        }
        // get back to original path
        else if (c==')'){
            dist = stack.back()[0];
            x    = stack.back()[1];
            y    = stack.back()[2];
            stack.pop_back();
        }
        // try different path
        else if (c=='|'){
            dist = stack.back()[0];
            x    = stack.back()[1];
            y    = stack.back()[2];
        }
        // take a step in the direction provided, then
        // add to grid of positions if not already visited
        // If already visited, update length if shorter than last visit
        else {
            x += (c=='E') - (c=='W');
            y += (c=='S') - (c=='N');
            dist++;
            if (grid.find(point(x,y))==grid.end() || dist < grid[point(x,y)]){
                grid[point(x,y)] = dist;
            }
        }
    }

    // find largest value in grid and num of values >= 1000
    int max = 0;
    int count = 0;
    for (const auto &pair : grid){
        max = std::max(max, pair.second);
        if (pair.second >= 1000){ count++; }
    }

    std::cout << "Answer (part 1): " << max << std::endl;
    std::cout << "Answer (part 2): " << count << std::endl;

    return 0;
}
