#ifndef GAME_H
#define GAME_H

#include<functional>
#include<vector>

//==============================================================================================
// Structs
//==============================================================================================
struct point{
   // parameters
   int x;
   int y;

   // constructors
   point(int x_, int y_): x(x_), y(y_){};
   point(const point &point_): x(point_.x), y(point_.y){};
   point(): x(0), y(0){};
};

struct unit{
   // parameters
   int hp;
   int attack;
   char type;
   point pos;


   // constructors
   unit(int attack_, char type_, point pos_): hp(200), attack(attack_), type(type_), pos(pos_){};
   unit(const unit &unit_): hp(unit_.hp), attack(unit_.attack), type(unit_.type), pos(unit_.pos){};
   unit(): hp(200), attack(3), type('X'){};
};

// sorting function for closest unit to p
struct distance_func{
   point p;

   distance_func(const point &p_): p(p_){};

   bool operator()(const point &lhs, const point &rhs);
};

// sorting function for lowest hp unit iterator
struct lowest_hp{
   bool operator()(const std::vector<unit>::iterator &lhs, const std::vector<unit>::iterator &rhs);
};

//==============================================================================================
// Operators and Hash functions
//==============================================================================================

// point addition
point operator+ (const point &lhs, const point &rhs);

// point equality
bool operator==(const point &lhs, const point &rhs);

// point sort
bool operator< (const point &lhs, const point &rhs);

// unit equality
bool operator==(const unit &lhs, const unit &rhs);

// unit sort
bool operator< (const unit &lhs, const unit &rhs);

// hash functoin for points
namespace std{
   template <> struct hash<point>{
      typedef point argument_type;
      typedef std::size_t result_type;
      std::size_t operator()(const point &pos) const noexcept{
         return std::hash<int>()(pos.x ^ (pos.y << 4));
      }
   };
}

//==============================================================================================
// Other functions
//==============================================================================================

// function to find valid neighbours of point
std::vector<point> find_neighbours(const point &current, const std::vector<std::string> &grid);

// check if current unit is adjacent to an enemy unit
bool next_to_enemy(const unit &current, std::vector<unit>::iterator &enemy_it, std::vector<unit> &units, const std::vector<std::string> &grid);

// check if any enemies are alive
bool enemies_alive(const unit &current, const std::vector<unit> &units);

// bfs pathfinding algorithm
std::unordered_map<point,point> bfs(const std::vector<std::string> &grid, const point &start, const point &finish);

// finds the closest open square to an enemy
std::vector<point> find_target_squares(const unit &subject ,const std::vector<unit> &units, const  std::vector<std::string> &grid);

// create a path from start to finish using map of point (map[point1]) leading to point (= point2)
std::vector<point> reconstruct_path(const point &start, const point &finish, std::unordered_map<point, point> &came_from);

#endif /* GAME_H */