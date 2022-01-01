#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<climits>
#include<limits>
#include<cstdlib>
#include<utility>
#include<queue>
#include"../../Utils/utils.h"

// function to compare nth element in vec of vec
struct sort_by_sr{
    int col;

    sort_by_sr(int col): col(col){};

    bool operator()(const std::vector<long long> &lhs, const std::vector<long long> &rhs){
        if (lhs[col]==rhs[col]){ return manhattan(lhs) < manhattan(rhs); }
        else {return lhs[col] < rhs[col]; }
    }
};

// pack bounding box properties together
struct box_prop{
    int nanobots;
    long long lattice_size;
    long long dist_to_orig;
    std::vector<long long> box_coord;

    box_prop(int nano, long long lat, long long dist, std::vector<long long> coord){
        nanobots     = nano;
        lattice_size = lat;
        dist_to_orig = dist;
        box_coord    = coord;
    }
};

// sorting func for bounding boxes (for prio queues, whick sort by >)
// sorts by contained bots -> box size -> distance to origin
struct box_sort{
    bool operator()(const box_prop &lhs, const box_prop &rhs){
        if (lhs.nanobots==rhs.nanobots){
            if (lhs.lattice_size==rhs.lattice_size){
                return lhs.dist_to_orig > rhs.dist_to_orig;
            }
            else { return lhs.lattice_size < rhs.lattice_size; }
        }
        else { return lhs.nanobots < rhs.nanobots; }
    }
};

// forward function declarations
int part1(const std::vector<std::vector<long long>> &input);
int part2(const std::vector<std::vector<long long>> &input);
int nano_in_box(const std::vector<std::vector<long long>> &input, const std::vector<long long> &box);

int main(){

    // read input into vector of vector of strings.
    std::vector<std::string> delimiters = {",","pos=<"," ",">","r="};
    std::vector<std::vector<long long>> input = input_to_llint_2D(read_input_2D("input_23", delimiters));

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

        std::vector<long long> bot1(nano.begin(),nano.begin()+3);
        std::vector<long long> bot2(it->begin(), it->begin()+3);

        if (manhattan(bot1,bot2) <= highest_radius){ count++; }       
    }

    return count;
}

int part2(const std::vector<std::vector<long long>> &input){

    std::vector<long long> best_coord(3);

    // create uniform box, lattice size multiple of 2, contaigning all points
    // find max coord in input
    long long bound = 0;
    for (const auto &line : input){
        for (int i=0; i<3; i++){
            bound = std::max(bound, std::abs(line[i]));
        }
    }

    long long max = 2;
    while (max < bound){ max*=2; }

    // create bounding box
    std::vector<long long> box = {-max,-max,-max, max, max, max};

    // initial box
    box_prop initial(input.size(), 2*max, max, box);

    // priority que of boxes, ordered by contained nanobots -> box size -> box dist to origin
    // We must keep tack of all boxes as we might divide the current best box and find that it becomes worse
    // than an older, less optimal box, seen previously
    std::priority_queue<box_prop, std::vector<box_prop>, box_sort> queue;
    
    // 8 octant coods in 3d space
    std::vector<std::vector<long long>> octants = {{0,0,0},{0,0,1},{0,1,0},{0,1,1},{1,0,0},{1,0,1},{1,1,0},{1,1,1}};

    queue.push(initial);

    while (!queue.empty()){
        
        box_prop current = queue.top();
        queue.pop();


        // if current lattice size is 1, found optimal coordinate
        if (current.lattice_size == 1){
            for (int i=0; i<3; i++){ best_coord[i] = current.box_coord[i]; }
            break;
        } 
        
        // reduce box size
        current.lattice_size /= 2;

        // split current box into octants and push new boxese onto queue
        for (const auto &octant : octants){
            std::vector<long long> new_box(6);

            for (int i=0; i<3; i++){
                new_box[i] = current.box_coord[i]+(current.lattice_size*octant[i]);
                new_box[i+3] = new_box[i] + current.lattice_size;
            }

            // find properties of new box
            int contains     = nano_in_box(input, new_box);
            int dist_to_orig = manhattan(new_box);

            // construct
            box_prop next(contains, current.lattice_size, dist_to_orig, new_box);

            // push to queue
            queue.push(next);
        }
    }

    return manhattan(best_coord);
}


// check if nanobot range intersects with box
int nano_in_box(const std::vector<std::vector<long long>> &input, const std::vector<long long> &box){

    int count = 0;
    for (const auto &line : input){
        
        long long d = 0LL;

        for (int i=0; i<3; i++){
            d += std::abs(line[i]-box[i]) + std::abs(line[i]-(box[i+3]-1));
            d -= (box[i+3]-1) - box[i];
        }

        d /= 2;

        if (d<=line[3]){ count++; }
    }

    return count;
}