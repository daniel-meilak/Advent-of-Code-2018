#include<iostream>
#include<vector>
#include<list>
#include<string>
#include<algorithm>
#include<cstdlib>
#include"../../Utils/utils.h"

int main(){

    // read input into vector of vector of strings.
    std::vector<std::string> delimiters = {","};
    std::vector<std::vector<int>> input = input_to_int_2D(read_input_2D("input_25", delimiters));

    // constellations list
    std::list<std::vector<std::vector<int>>> cons, copy;

    // work through input
    for (const std::vector<int> &star : input){

        // check if star has been added to a constellation
        bool put_in_con = false;

        // check if point connects to any current constellation
        for (std::vector<std::vector<int>> &con : cons){

            for (const std::vector<int> &s : con){
                if (manhattan(star,s)<=3){
                    con.push_back(star);
                    put_in_con = true;
                    break;
                }
            }
        }

        // if no current constellation can contain star
        // create new constellation
        if (!put_in_con){
            cons.push_back({star});
        }
    }

    copy = cons;

    // some constallations are not fully linked up
    // loop back through constellations, linking them together
    do {
        copy = cons;

        for (auto con_it1=cons.begin(); con_it1!=cons.end()--; con_it1++){
            for (auto con_it2=std::next(con_it1); con_it2!=cons.end(); con_it2++){

                bool connection = false;

                // loop through all star pairs in *con_it1 and *con_it2
                for (auto star_it1=con_it1->begin(); star_it1!=con_it1->end(); star_it1++){
                    for (auto star_it2=con_it2->begin(); star_it2!=con_it2->end(); star_it2++){

                        // if any two stars in the two constellations connect, combine
                        if (manhattan((*star_it1),(*star_it2))<=3){

                            // add second con to first
                            con_it1->insert(con_it1->end(), con_it2->begin(), con_it2->end());

                            // delete second con from cons
                            con_it2 = cons.erase(con_it2);
                            if (con_it2==cons.end()){ con_it2--; }
                            connection = true;
                        }

                        if (connection){ break; }
                    }
                    if (connection){ break; }
                }
            }
        }
    } while (copy != cons);


    std::cout << "Answer (part 1): " << cons.size() << std::endl;

    return 0;
}


