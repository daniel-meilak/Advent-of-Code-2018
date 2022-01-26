#include<iostream>
#include<vector>
#include<queue>
#include<string>
#include<algorithm>
#include<cstdlib>
#include<utility>
#include<unordered_map>
#include"utils.h"

// point struct, includes tool used on point
struct point{
    int x,y;
    char tool;

    // parametrised constructor with tool
    point(int x_, int y_, char tool_): x(x_), y(y_), tool(tool_){}; 

    // parametrised constructor without tool
    point(int x_, int y_): x(x_), y(y_){};

    // default constructor
    point(): x(0), y(0), tool('X'){};
};

// point equality
bool operator==(const point &lhs, const point &rhs){
    return (lhs.x==rhs.x && lhs.y==rhs.y && lhs.tool==rhs.tool);
}

// point nequality
bool operator!=(const point &lhs, const point &rhs){
    return (lhs.x!=rhs.x || lhs.y!=rhs.y);
}

// point addition
point operator+ (const point &lhs, const point &rhs){
    return point(lhs.x+rhs.x, lhs.y+rhs.y);
}

// greater than operator for priority queue
bool operator> (const std::pair<point,int> &lhs, const std::pair<point,int> &rhs){
    return lhs.second > rhs.second;
}

// forward function declarations
std::string hasher(const point &p);
int find_cost(const point &start, const point &end);
std::vector<point> find_neighbours(const point &p, const std::vector<std::vector<char>> &grid);

int main(){

    // read input
    std::vector<std::string> delimiters = {"depth: ","target: ",","};
    std::vector<std::vector<int>> input = input_to_int_2D(read_input_2D("input_22",delimiters));

    const int depth    = input[0][0];
    const int width    = 1000;
    const int target_x = input[1][0];
    const int target_y = input[1][1];

    // grid of cave system
    std::vector<std::vector<char>> caves(depth, std::vector<char>(width,'.'));

    // grid of associated erosion levels
    std::vector<std::vector<int>> erosion_levels(depth, std::vector<int>(width));

    // find type of each region in caves
    for (int y=0; y<depth; y++){
        for (int x=0; x<width; x++){

            // find geologic index
            int geo_index;

            // origin and target have geological index of 0
            if ( (x==0 && y==0)|| (x==target_x && y==target_y) ){ geo_index = 0; }
            // top border
            else if (y==0){ geo_index = x*16807; }
            // left border
            else if (x==0){ geo_index = y*48271; }
            // all other squares
            else { geo_index = erosion_levels[y][x-1]*erosion_levels[y-1][x]; }

            // find erosion level
            erosion_levels[y][x] = (geo_index + depth) % 20183;

            // determine type
            int result = erosion_levels[y][x] % 3;
            if      (result==0){ caves[y][x]='.'; } // rocky
            else if (result==1){ caves[y][x]='='; } // wet
            else    /*res==2 */{ caves[y][x]='|'; } // narrow
        }
    }

    // determine risk level from origin to target
    int risk_level = 0;
    for (int x=0; x<=target_x; x++){
        for (int y=0; y<=target_y; y++){

            if      (caves[y][x]=='='){ risk_level+=1; }
            else if (caves[y][x]=='|'){ risk_level+=2; }
        }
    }

    // part 2, Dijkstra algorithm
    // start and end positions, map of path and cost
    point start(0,0,'T');
    point end(target_x,target_y,'T');
    std::unordered_map<std::string,point> came_from;
    std::unordered_map<std::string,int> cost_so_far;

    // priority queue
    std::priority_queue<std::pair<point,int>, std::vector<std::pair<point,int>>,std::greater<std::pair<point,int>>> frontier; 
    frontier.emplace(start,0);

    came_from[hasher(start)] = start;
    cost_so_far[hasher(start)] = 0;

    // pathfinding loop
    while (!frontier.empty()){

        // get element in queue with lowest weight
        point current = frontier.top().first;
        frontier.pop();

        // if current element is the end, exit loop
        if (current==end){ break; }

        // find next steps from current
        for (const point &next : find_neighbours(current,caves)){

            // find the weight of next step
            int new_cost = cost_so_far[hasher(current)] + find_cost(current,next);

            std::string next_s = hasher(next);
            if ( cost_so_far.find(next_s)==cost_so_far.end() || new_cost < cost_so_far[next_s] ){
                cost_so_far[next_s] = new_cost;
                came_from[next_s]   = current;
                frontier.emplace(next, new_cost);
            }   
        }
    }

    std::cout << "Answer (part 1): " << risk_level               << std::endl;
    std::cout << "Answer (part 2): " << cost_so_far[hasher(end)] << std::endl;

    return 0;
}

// very basic hashing function for points
std::string hasher(const point &p){
    return std::to_string(p.x)+","+std::to_string(p.y)+","+p.tool;
}

// find all neighbours of point in graph
std::vector<point> find_neighbours(const point &p, const std::vector<std::vector<char>> &grid){

    // graph height and width
    size_t height = grid.size();
    size_t width  = grid[0].size(); 

    // Right, Left, South, North neighbours
    std::vector<point> directions = {point(1,0), point(-1,0), point(0,1), point(0,-1)};

    // vector of neighbours
    std::vector<point> neighbours;

    for (const point &dir : directions){

        point n = p + dir;

        // must be a valid position
        if (n.x>=0 && n.y>=0 && n.x<(int)width && n.y<(int)height){

            // if rocky, can use climbing gear 'G' or torch 'T'
            if (grid[n.y][n.x]=='.'){
                // cannot equip gear in narrow
                if (grid[p.y][p.x]!='|'){
                    n.tool = 'G';
                    neighbours.push_back(n);
                }
                // cannot equip torch in wet
                if (grid[p.y][p.x]!='='){
                    n.tool = 'T';
                    neighbours.push_back(n);
                }
            }
            // if wet, can use climbing gear 'G' or nothing 'N'
            else if (grid[n.y][n.x]=='='){
                // cannot equip grea in narrow
                if (grid[p.y][p.x]!='|'){
                    n.tool = 'G';
                    neighbours.push_back(n);
                }
                // cannot equip nothing in rocky
                if (grid[p.y][p.x]!='.'){
                    n.tool = 'N';
                    neighbours.push_back(n);
                }
            }
            // if narrow, can use torch 'T' or nothing 'N'
            else if (grid[n.y][n.x]=='|'){
                // cannot equip torch in wet
                if (grid[p.y][p.x]!='='){
                    n.tool = 'T';
                    neighbours.push_back(n);
                }
                // cannot equip nothing in rocky
                if (grid[p.y][p.x]!='.'){
                    n.tool = 'N';
                    neighbours.push_back(n);
                }
            }
        }
    }

    return neighbours;
}

// the cost of moving form two adjacent tiles start and end
int find_cost(const point &start, const point &end){
    
    // if no change of tool needed
    if (start.tool==end.tool){ return 1; }
    // if change of tool is needed
    else { return 8; }
}