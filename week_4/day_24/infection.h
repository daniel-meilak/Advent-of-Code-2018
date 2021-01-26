#ifndef INFECTION_H
#define INFECTION_H

#include<string>
#include<vector>
#include<algorithm>

//======================================================================
// Structs
//======================================================================

struct army_t{
   int id;
   int target;
   char army_type;
   int units;
   int hp;
   int attack;
   std::string damage_type;
   int initiative;
   std::vector<std::string> weak, immune;
};

// sorting struct for best target for unit
struct target_sort{
   army_t targeter;

   target_sort(army_t targeter_): targeter(targeter_){};

   bool operator()(const army_t &lhs, const army_t &rhs);
};

//======================================================================
// Sorting functions
//======================================================================

// target selection sorted by effective power -> initiative
bool selection_order(const army_t &lhs, const army_t &rhs);

bool initiative(const army_t &lhs, const army_t &rhs);

//======================================================================
// Other functions
//======================================================================

void target_selection(std::vector<army_t> &units);

void attack_phase(std::vector<army_t> &units);

int damage_calc(const army_t &from, const army_t &to);

#endif /* INFECTION_H */ 