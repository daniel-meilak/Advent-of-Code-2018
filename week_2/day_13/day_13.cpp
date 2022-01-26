#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<ranges> // C++20
#include<cstdlib>
#include"utils.h"

struct cart{

    int x;
    int y;
    int dir;
    int step = 0;

    cart(int x, int y, int dir): x(x), y(y), dir(dir){};
};

bool sort_by_pos(const cart &lhs, const cart &rhs){
    if (lhs.y==rhs.y){ return lhs.x < rhs.x; }
    else { return lhs.y < rhs.y; }
}

int main(){

    // read input into vector of strings.
    std::vector<std::string> input = read_input("input_13");

    // vector of carts
    std::vector<cart> carts;

    // first find all carts and replace with track
    size_t size_y = input.size();
    size_t size_x = input[0].size();
    for (size_t x=0; x<size_x; x++){
        for (size_t y=0; y<size_y; y++){

            char &c = input[y][x]; 

            switch(c){
                case('>'):
                    carts.push_back(cart(x,y,0));
                    input[y][x]='-';
                    break;
                case('<'):
                    carts.push_back(cart(x,y,180));
                    input[y][x]='-';
                    break;
                case('^'):
                    carts.push_back(cart(x,y,90));
                    input[y][x]='|';
                    break;
                case('v'):
                    carts.push_back(cart(x,y,270));
                    input[y][x]='|';
                    break;
            }
        }
    }

    // keep initial cart number
    size_t total_carts = carts.size();

    // collision coordinates
    int col_x=0, col_y=0;

    // move the carts and look for collisions
    while(carts.size()>1){

        // sort carts from top left to bottom right
        std::ranges::sort(carts, sort_by_pos);

        for (size_t j=0; j<carts.size(); j++){
            cart &c = carts[j];

            // move in current direction
            switch(c.dir){
                case(90):
                    c.y--;
                    break;
                case(270):
                    c.y++;
                    break;
                case(180):
                    c.x--;
                    break;
                case(0):
                    c.x++;
                    break;
            }

            // check next square
            char &square = input[c.y][c.x];
            // intersection
            if (square=='+'){
                switch(c.step){
                    case(0):
                        c.dir  = mod(c.dir+90,360);
                        c.step = 1;
                        break;
                    case(1):
                        c.step = 2;
                        break;
                    case(2):
                        c.dir  = mod(c.dir-90,360);
                        c.step = 0;
                        break;
                }
            }
            // corner
            else if (square=='\\'){
                switch(c.dir){
                    case(0):
                        c.dir = 270;
                        break;
                    case(90):
                        c.dir = 180;
                        break;
                    case(180):
                        c.dir = 90;
                        break;
                    case(270):
                        c.dir = 0;
                        break;
                }
            }
            // corner
            else if (square=='/'){
                switch(c.dir){
                    case(0):
                        c.dir = 90;
                        break;
                    case(90):
                        c.dir = 0;
                        break;
                    case(180):
                        c.dir = 270;
                        break;
                    case(270):
                        c.dir = 180;
                        break;
                }
            }

            // check for collision
            for (size_t k=0; k<carts.size(); k++){
                // dont compare cart to itself
                if (k==j){ continue; }
                    
                if (c.x==carts[k].x && c.y==carts[k].y){

                    // part 1, keep pos of first collision
                    if (carts.size()==total_carts){
                        col_x = c.x;
                        col_y = c.y;
                    }

                    // part 2, remove colliding carts
                    carts.erase(carts.begin()+k);
                    if (k<j){
                        carts.erase(carts.begin()+j-1);
                        j--;
                    }
                    else { carts.erase(carts.begin()+j); }
                    j--;
                    break;

                }
            }  
        }
    }

    std::cout << "Answer (part 1): " << col_x      << "," << col_y      << std::endl;
    std::cout << "Answer (part 2): " << carts[0].x << "," << carts[0].y << std::endl;

    return 0;
}
