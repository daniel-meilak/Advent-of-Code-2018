#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<cstdlib>
#include<climits>
#include<ranges> // C++20
#include"game.h"
#include"../../Utils/utils.h"

// forward function declaration
int resolve(std::vector<std::string> input, const int elves_attack);

int main(){

    // read input into vector of strings.
    std::vector<std::string> input = read_input("input", "");

    std::cout << "Answer (part 1): " << resolve(input,3) << std::endl;
    std::cout << "Answer (part 2): " << resolve(input,23) << std::endl;

    return 0;
}

int resolve(std::vector<std::string> input, const int elves_attack){

    // vector of elves and goblins
    std::vector<unit> units;

    // attack of elves
    int attack = elves_attack;

    // num of elves
    int elves = 0;

    // find positions of elves and goblins
    size_t width  = input[0].size();
    size_t height = input.size();
    for (size_t x=0; x<width; x++){
        for (size_t y=0; y<height; y++){
            
            char &c = input[y][x];

            if (c=='E'){
                units.push_back(unit(attack,c,point(x,y)));
                elves++;
            }
            else if (c=='G'){
                units.push_back(unit(3,c,point(x,y)));
            }
        }
    }

    // flag for end of combat
    bool fighting = true;

    // rounds count
    int rounds = 0;

    std::vector<std::string> last_round = input;

    while (fighting){

        // sort units in fighting order
        std::sort(units.begin(), units.end());

        // run rounds of movement and combat
        for (size_t i=0; i<units.size(); i++){

            // current unit and iterator to enemy
            unit &u = units[i];
            std::vector<unit>::iterator enemy_it;

            // first check that there are enemies alive
            if (!enemies_alive(u, units)){
                // if none are, end round and fight
                fighting = false;
                break;
            }

            // if not already next to enemy, move towards closest
            if (!next_to_enemy(u,enemy_it,units,input)){

                // find closest open enemy squares to unit
                std::vector<point> closest = find_target_squares(u, units, input);

                // next square
                point next = u.pos;
                unsigned int path_length = UINT_MAX;
                for (const point &target : closest){

                    // find shortest path to target
                    std::unordered_map<point, point> path = bfs(input,u.pos,target);

                    // if target cant be reached, try next target
                    if (path.find(target)==path.end()){ continue; }

                    // find path to current target
                    std::vector<point> possible_path = reconstruct_path(u.pos,target,path);

                    // if path length is equal to current shortest path
                    if (possible_path.size() == path_length){

                        // check if first step in new path is higher prio (reading order)
                        if (possible_path[0]<next){ next = possible_path[0]; }
                    }
                    // if path length is shorter that current shortest path
                    else if (possible_path.size() < path_length){

                        // choose its first step
                        next = possible_path[0];
                        path_length = possible_path.size();
                    }
                }

                // remove marker from grid
                input[u.pos.y][u.pos.x] = '.';

                // move unit to next pos
                u.pos = next;

                // add updated marker to grid
                input[u.pos.y][u.pos.x] = u.type;
            }

            // if next to enemy with/without movement, attack
            if (next_to_enemy(u,enemy_it,units,input)){

                // reduce enemy hp by u.attack
                enemy_it->hp -= u.attack;

                // if enemy hp falls below zero, enemy dies, update grid
                if (enemy_it->hp <= 0){
                    input[enemy_it->pos.y][enemy_it->pos.x] = '.';

                    // remove from units and update index i
                    // if enemy appears before u, decrement i
                    if (*enemy_it < u){ i--; }
                    units.erase(enemy_it);
                }
            }
        }

        last_round = input;

        // increment round counter
        if (fighting){ rounds++; }
    }

    // sum of units hp
    int leftover_hp = 0;
    for (const unit &u : units){
        leftover_hp += u.hp;
    }

    // std::cout << "Started with " << elves << " elves" << std::endl;
    // std::cout << "Units left after combat: " << units.size() << std::endl;
    // std::cout << "Outcome: ";

    return rounds*leftover_hp;
}