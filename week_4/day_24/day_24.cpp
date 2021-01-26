#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<cstdlib>
#include"../../Utils/utils.h"
#include"infection.h"

// forward function declarations
std::vector<army_t> create_units(const std::vector<std::vector<std::string>> &input);
int total_units(const std::vector<army_t> &units);
bool war_over(const std::vector<army_t> &units);
int fight_war(std::vector<army_t> units, const int &boost, char &winner);

int main(){

    // read input into vector of vector of strings.
    std::vector<std::string> delimiters = {" ","units each with","hit points","(",")",",",";","to","an attack that does","damage at",":"};
    std::vector<std::vector<std::string>> input = read_input_2D("input", delimiters);

    // digest input
    std::vector<army_t> units = create_units(input);

    // check who won
    char winner = '0';

    std::cout << "Answer (part 1): " << fight_war(units,0,winner) << std::endl;

    // part 2
    // brute force increace boost until immune system wins
    int boost = 0;
    int count = 0;
    while (winner!='A'){
        // reset winner
        winner = '0';

        count = fight_war(units,boost,winner);
        boost++;
    }

    std::cout << "Answer (part 2): " << count <<  std::endl;

    return 0;
}

int fight_war(std::vector<army_t> units, const int &boost, char &winner){

    // apply boost to Immune system ('A')
    if (boost>0){
        for (auto &u : units){
            if (u.army_type=='A'){ u.attack += boost; }
        }
    }
   
    while (!war_over(units)){
        // war can also end if no units take damage
        int total = total_units(units);

        // choose target for each unit
        target_selection(units);

        // units attack their targets
        attack_phase(units);

        // check that num units has changed
        if (total == total_units(units)){
            winner = 'D';
            break;
        }
    }

    // if not draw, get winning side type
    if (winner!='D'){ winner = units[0].army_type; }

    return total_units(units);
}

std::vector<army_t> create_units(const std::vector<std::vector<std::string>> &input){
    
    // vector to store units
    std::vector<army_t> units;

    // digest input
    char army_type = '0';
    int id = 1;
    for (const auto &line : input){
        if (line.empty()){continue; }
        else if (line[0]=="Immune"   ){
            army_type='A';
            continue;
        }
        else if (line[0]=="Infection"){
            army_type='X';
            continue;
        }  

        // tmp army to store info
        army_t tmp;
        tmp.army_type = army_type;
        tmp.units     = std::stoi(line[0]);
        tmp.hp        = std::stoi(line[1]);
        tmp.id        = id;

        for (unsigned int i=2; i<line.size(); i++){
            if (line[i]=="weak"){
                while (line[i+1]!="immune" && line[i+1]!="with"){
                    tmp.weak.push_back(line[i+1]);
                    i++;
                }
            }
            else if (line[i]=="immune"){
                while (line[i+1]!="with" && line[i+1]!="weak"){
                    tmp.immune.push_back(line[i+1]);
                    i++;
                }
            }
            else if (line[i]=="with"){
                tmp.attack      = std::stoi(line[i+1]);
                tmp.damage_type = line[i+2];
                i+=2;
            }
            else if (line[i]=="initiative"){
                tmp.initiative = std::stoi(line[i+1]);
                i++;
            }
            else {
                std::cout << "Error reading" << std::endl;
                std::exit(EXIT_FAILURE);
            }
        }

        // add to units
        units.push_back(tmp);

        // increment id for next unit
        id++;
    }

    return units;
}

// check if war is over by looking for one army type in all units
bool war_over(const std::vector<army_t> &units){

    char side = units.front().army_type;

    for (const auto &unit : units){
        if (unit.army_type!=side){
            return false;
        }
    }

    return true;
}

// find total unit count
int total_units(const std::vector<army_t> &units){

    int total = 0;
    for (const army_t &unit : units){
        total += unit.units;
    }

    return total;
}
