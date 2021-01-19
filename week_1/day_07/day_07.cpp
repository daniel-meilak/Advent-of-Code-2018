#include<iostream>
#include<vector>
#include<list>
#include<string>
#include<algorithm>
#include<unordered_map>
#include<utility>
#include<cstdlib>
#include"../../Utils/utils.h"

struct step{
    bool finished = false;
    std::string requirements;
    std::string required_for;
};

struct worker{
    bool working = false;
    int timer;
    char job;
};

// worker sorting function
bool sort_by_job(const worker &lhs, const worker &rhs){
    return lhs.job < rhs.job;
}

// forward function declarations
std::string part1(std::unordered_map<char,step> steps, std::list<char> queue, std::string unique, std::string path);
int part2(std::unordered_map<char,step> steps, std::list<char> queue, std::string unique, std::string path, const int &n);

int main(){

    // read input into vector of vector of strings.
    std::vector<std::string> delimiters = {"Step "," must be finished before step "," can begin."};
    std::vector<std::vector<std::string>> input = read_input_2D("input", delimiters);

    // map of steps
    std::unordered_map<char, step> steps;

    for (const auto &line : input){
        
        steps[line[0][0]].required_for += line[1][0];
        steps[line[1][0]].requirements += line[0][0];
    }

    // queue and for BFS
    std::list<char> queue;
    

    // find the starting points
    for (const auto &pair : steps){
        if (pair.second.requirements.size() == 0){
            queue.push_back(pair.first);
        }
    }

    // unique steps for BFS and path
    std::string unique(queue.begin(), queue.end());
    std::string path;

    // number of workers for part 2
    const int workers = 5;

    std::cout << "Answer (part 1): " << part1(steps,queue,unique,path)         << std::endl;
    std::cout << "Answer (part 2): " << part2(steps,queue,unique,path,workers) << std::endl;

    return 0;
}

std::string part1(std::unordered_map<char,step> steps, std::list<char> queue, std::string unique, std::string path){
    
    // start BFS
    while (queue.size()){

        // queue must be sorted alphabetically
        queue.sort();

        // look for first memb in queue with all requrements
        auto it = queue.begin();
        bool req = true;
        while(req){
            req = false;

            // check each requirement
            for (const char &requirement : steps[*it].requirements ){

                // if any is not met, repeat loop with next member in queue
                if (steps[requirement].finished==false){
                    req = true;
                    it++;
                    break;
                }
            }
        }

        // add all unique steps that require current to queue
        for (const char &c : steps[*it].required_for){
            if (std::find(unique.begin(), unique.end(), c)==unique.end()){
                queue.push_back(c);
                unique.push_back(c);
            }
        }

        // add current step to path, set finished to true and remove from queue
        steps[*it].finished = true;
        path += *it;        
        queue.erase(it);
    }

    return path;
}

int part2(std::unordered_map<char,step> steps, std::list<char> queue, std::string unique, std::string path, const int &n){
    
    // five workers
    std::vector<worker> workers(n);

    // flag for working workers
    bool workers_working = false;

    // time to complete
    int time = 0;

    // start BFS
    while (queue.size() || workers_working){

        // queue must be sorted alphabetically
        queue.sort();

        // check if workers need jobs
        for (auto &w : workers){

            // if idle find job
            if (w.working==false){

                // look for first memb in queue with all requrements met
                auto it = queue.begin();
                bool req = true;
                while(req){

                    // if no jobs are left for worker, break
                    if (it==queue.end()){ break; }

                    req = false;

                    // check each requirement
                    for (const char &requirement : steps[*it].requirements ){

                        // if any is not met, repeat loop with next member in queue
                        if (steps[requirement].finished==false){
                            req = true;
                            it++;
                            break;
                        }
                    }
                }

                // if we found a job, assign to worker
                if (it!=queue.end()){
                    w.job     = *it;
                    w.timer   = 60 + *it + 1 - 'A';
                    w.working = true;

                    // remove job from queue
                    queue.erase(it);
                }
            }
        }

        // workers work for a second, reduce timers
        // (doenst matter if idle goes to negative time )
        for (auto &w : workers){
            w.timer--;
        }

        // sort workers by their jobs incase two finish at same time
        std::sort(workers.begin(), workers.end(), sort_by_job);

        // check if any worker is finished
        for (auto &w : workers){
            
            // worker is finished, add next jobs to queue
            if (w.timer==0){

                // add all unfinished steps that require current workers job to queue
                for (const char &c : steps[w.job].required_for){
                    if (std::find(unique.begin(), unique.end(), c)==unique.end()){
                        queue.push_back(c);
                        unique.push_back(c);
                    }
                }

                // add job to path and flag job as finished
                path += w.job;
                steps[w.job].finished = true;

                // set worker to not working
                w.working = false;
            }
        }

        // if any worker is working set flag
        workers_working = false;
        for (const auto &w : workers){
            if (w.working==true){
                workers_working = true;
                break;
            }
        }

        time++;
    }

    return time;
}