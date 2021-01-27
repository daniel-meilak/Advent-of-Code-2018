#include"game.h"
#include"../../Utils/utils.h"

#include<vector>
#include<queue>
#include<unordered_map>

//==============================================================================================
// Struct member functions
//==============================================================================================
bool distance_func::operator()(const point &lhs, const point &rhs){
   // distances to target point
   int d1 = manhattan(lhs.x, lhs.y, p.x, p.y);
   int d2 = manhattan(rhs.x, rhs.y, p.x, p.y);

   // if distance is equal, choose most north west point
   if (d1==d2){ return lhs < rhs; }
   else { return d1 < d2; }
}

bool lowest_hp::operator()(const std::vector<unit>::iterator &lhs, const std::vector<unit>::iterator &rhs){
   if (lhs->hp==rhs->hp){ return lhs->pos < rhs->pos; }
   else { return lhs->hp < rhs->hp; }
}

//==============================================================================================
// Operators
//==============================================================================================

// point addition
point operator+ (const point &lhs, const point &rhs){
   return point(lhs.x+rhs.x, lhs.y+rhs.y);
}

// point equality
bool operator==(const point &lhs, const point &rhs){
   return (lhs.x==rhs.x && lhs.y==rhs.y);
}

// point difference
bool operator!=(const point &lhs, const point &rhs){
   return (lhs.x!=rhs.x || lhs.y!=rhs.y);
}

// point sort
bool operator< (const point &lhs, const point &rhs){
   if (lhs.y==rhs.y){ return lhs.x < rhs.x; }
   else { return lhs.y < rhs.y; }
}

// unit equality
bool operator==(const unit &lhs, const unit &rhs){
   return lhs.pos==rhs.pos;
}

// unit sort
bool operator< (const unit &lhs, const unit &rhs){
   return lhs.pos < rhs.pos;
}

//==============================================================================================
// Other functions
//==============================================================================================

// function to find valid neighbours of point
std::vector<point> find_neighbours(const point &current, const std::vector<std::string> &grid){

   // North, West, East and South neighbours
   std::vector<point> dirs = {point(0,-1), point(-1,0), point(1,0), point(0,1)}; 

   // hold valid neighbours
   std::vector<point> output;

   for (const point &dir : dirs){
      point n = current + dir;

      // validate neighbour, cannot be wall, or other entity
      if (grid[n.y][n.x]=='.'){
         output.push_back(n);
      }
   }

   return output;
}

// check if current unit is adjacent to an enemy unit
bool next_to_enemy(const unit &current, std::vector<unit>::iterator &enemy_it, std::vector<unit> &units, const std::vector<std::string> &grid){

   // enemy type
   char enemy_type;
   if (current.type=='E'){ enemy_type='G'; }
   else { enemy_type='E'; }

   // vector of iterators to all neighbouring enemies
   std::vector<std::vector<unit>::iterator> enemies;

   // North, West, East and South neighbours
   std::vector<point> dirs = {point(0,1), point(-1,0), point(1,0), point(0,-1)};

   for (const point &dir : dirs){
      point n = current.pos + dir;

      // look for enemy type in neighbour dir
      if (grid[n.y][n.x]==enemy_type){

         // keep track of enemy
         for (auto it=units.begin(); it!=units.end(); it++){
            if (it->pos.x==n.x && it->pos.y==n.y){
               enemies.push_back(it);
               break;
            }
         }
      }
   }

   // if no neighbouring enemies
   if (enemies.empty()){
      return false;
   }
   // if one neighbouring enemy
   else if (enemies.size()==1){
      enemy_it = enemies.front();
      return true;
   }
   // if multiple enemies, find lowest hp or reading order enemy
   else {

      // sort by hp then reading order
      std::sort(enemies.begin(), enemies.end(), lowest_hp());
      enemy_it = enemies.front();
      return true;
   }
}

// check if any enemies are alive
bool enemies_alive(const unit &current, const std::vector<unit> &units){

   // enemy type
   char enemy_type;
   if (current.type=='E'){ enemy_type='G'; }
   else { enemy_type='E'; }

   for (const unit &u : units){
      if (u.type==enemy_type){ return true; }
   }

   return false;
}

std::unordered_map<point,point> bfs(const std::vector<std::string> &grid, const point &start, const point &finish){

   // queue to process nodes starting from 'start'
   std::queue<point> frontier;
   frontier.push(start);

   // unordered map to recreate path
   std::unordered_map<point,point> came_from;
   came_from[start] = start;

   // work through queue untill we find end
   while (!frontier.empty()){

      // start at beginning of queue and pop front
      point current = frontier.front();
      frontier.pop();

      // stop if the current point is the goal
      if (current == finish){ break; }

      // generate neighbours for current point
      std::vector<point> neighbours = find_neighbours(current, grid);
      for (point next : neighbours){

         // if neighbour hasn't been visited already
         if (came_from.find(next) == came_from.end()){

            // add to queue and path
            frontier.push(next);
            came_from[next] = current;
         }
      }
   }

   return came_from;
}

// finds the closest open square to an enemy
std::vector<point> find_target_squares(const unit &subject ,const std::vector<unit> &units, const  std::vector<std::string> &grid){

   char ally_type = subject.type;

   // vector of open squares
   std::vector<point> open_squares;

   // compile vector of neighbours
   for (const unit &u : units){

      // ignore allies
      if (u.type==ally_type){ continue; }

      // find all valid neighbouring squares
      std::vector<point> neighbours = find_neighbours(u.pos, grid);

      // add to full list
      for (const point &p : neighbours){
         open_squares.push_back(p);
      }
   }

   // sort all open squares according to closest
   std::sort(open_squares.begin(), open_squares.end(), distance_func(subject.pos));

   // return closest
   return open_squares;
}

// create a path from start to finish using map of point (map[point1]) leading to point (= point2)
std::vector<point> reconstruct_path(const point &start, const point &finish, std::unordered_map<point, point> &came_from){

   std::vector<point> path;

   point current = finish;

   // reconstruct from finish to start
   while (current != start){
      path.push_back(current);
      current = came_from[current];
   }

   // reverse to start at 'start'
   std::reverse(path.begin(), path.end());

   return path;
}