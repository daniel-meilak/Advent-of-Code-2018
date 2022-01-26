#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<cstdlib>
#include<utility>
#include<unordered_map>
#include"utils.h"

int main(){

    // read input into vector of vector of strings.
    std::vector<std::string> delimiters = {" ","#","[","]",":"};
    std::vector<std::vector<std::string>> input = read_input_2D("input_04", delimiters);

    // format is sorted automatically!
    std::sort(input.begin(), input.end());

    // map of guard ID to sleep times
    // <guard id, <total time slept, <minute, time slept at minute>>>
    std::unordered_map<int,std::pair<int,std::unordered_map<int,int>>> guards;

    // work through input
    int id;
    int sleep_start;
    int sleep_end;
    for (const auto &line : input){

        const std::string &key = line[3];

        if      (key=="Guard"){ id          = std::stoi(line[4]); }
        else if (key=="falls"){ sleep_start = std::stoi(line[2]); }
        else if (key=="wakes"){
            sleep_end = std::stoi(line[2]);

            // keep track of sleep time
            for (int i=sleep_start; i<sleep_end; i++){

                // increment sleep time at minute i
                guards[id].second[i]++;

                // increment overall sleep time
                guards[id].first++;
            }      
        }
    }

    // find id of sleepiest guard
    int sleepiest_id;
    int time = 0;
    for (const auto &id : guards){
        if (id.second.first > time){
            sleepiest_id = id.first;
            time = id.second.first;
        }
    }

    // find sleepiest hour of sleepies guard
    int sleepiest_minute = 0;
    time = 0;
    for (const auto &t : guards[sleepiest_id].second){
        if (t.second > time){
            sleepiest_minute = t.first;
            time = t.second;
        }
    }

    // find the minute a guard was asleep for the longest
    int highest_minute{0};
    int worst_guard{0};
    time = 0;
    for (const auto &id : guards){
        for (const auto &t : id.second.second){
            if (t.second > time){
                highest_minute = t.first;
                worst_guard = id.first;
                time = t.second;
            }
        }
    }

    std::cout << "Answer (part 1): " << sleepiest_minute*sleepiest_id << std::endl;
    std::cout << "Answer (part 2): " << highest_minute*worst_guard    << std::endl;

    return 0;
}
