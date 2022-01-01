#pragma once

#include<string>
#include<vector>
#include<algorithm>
#include<iostream>

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
// Other functions
//======================================================================

int damage_calc(const army_t &from, const army_t &to){

   int effective_attack = from.attack*from.units;
   const std::vector<std::string> &im = to.immune;
   const std::vector<std::string> &w  = to.weak;
   const std::string &dt = from.damage_type;

   if (std::find(im.begin(), im.end(), dt) != im.end()){ return 0; }
   else if (std::find(w.begin(), w.end(), dt) != w.end()){ return 2*effective_attack; }
   else { return effective_attack; }
}

//======================================================================
// Sorting functions
//======================================================================

// target selection sorted by effective power -> initiative
bool selection_order(const army_t &lhs, const army_t &rhs){
   int lhs_eff_pow = lhs.units * lhs.attack;
   int rhs_eff_pow = rhs.units * rhs.attack;

   if (lhs_eff_pow==rhs_eff_pow){ return lhs.initiative > rhs.initiative; }
   else { return lhs_eff_pow > rhs_eff_pow; }
}

bool initiative(const army_t &lhs, const army_t &rhs){
   return lhs.initiative > rhs.initiative;
}

bool target_sort::operator()(const army_t &lhs, const army_t &rhs){
   int damage_to_lhs = damage_calc(targeter,lhs);
   int damage_to_rhs = damage_calc(targeter,rhs);
   int eff_pow_lhs   = lhs.units * lhs.attack;
   int eff_pow_rhs   = rhs.units * rhs.attack;

   if (damage_to_lhs==damage_to_rhs){
      if (eff_pow_lhs==eff_pow_rhs){
         return lhs.initiative > rhs.initiative;
      }
      else {return eff_pow_lhs > eff_pow_rhs; }
   }
   else {return damage_to_lhs > damage_to_rhs; }
}

//======================================================================
// Other functions
//======================================================================

void target_selection(std::vector<army_t> &units){

   // sort into target selection order
   std::sort(units.begin(), units.end(), selection_order);

   // vector of already targeted armies
   std::vector<int> taken;

   // find target for each unit
   for (army_t &unit : units){

      // compile list of possible targets
      std::vector<army_t> targets;
      for (army_t &u : units){

         // cant target itself, allies or taken units
         if (u.army_type==unit.army_type || std::find(taken.begin(), taken.end(), u.id)!=taken.end()){ continue; }

         // if possible to deal damage, add to list
         if (damage_calc(unit,u)!=0){ targets.push_back(u); }         
      }

      // sort eligible targets by damage given -> effective power -> initiative
      std::sort(targets.begin(), targets.end(), target_sort(unit));

      // assign target id to unit and add to taken list
      if (targets.size()==0){ unit.target = 0; }
      else {
         unit.target = targets.front().id;
         taken.push_back(targets.front().id);
      }
   }
}

void attack_phase(std::vector<army_t> &units){

   // sort in attack order
   std::sort(units.begin(), units.end(), initiative);

   for (army_t &attacker : units){

      // don't allow dead units to attack
      if (attacker.units <= 0){ continue; }

      // find target
      for (army_t &defender : units){

         // deal damage to target and calc leftover units
         if (defender.id == attacker.target){

            defender.units -= damage_calc(attacker,defender)/defender.hp;
            break;
         }
      }
   }

   // remove any empty units from battle
   for (auto it=units.begin(); it!=units.end();){

      if (it->units <= 0){ it = units.erase(it); }
      else { it++; }
   }
}

