#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <map>
#include <string>
#include "SDL.h"

class Game;
enum class CellType;

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };
  enum class Type { human, machine };

  Snake(int grid_width, int grid_height)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2) {}

  Snake(const Snake& s);
  Snake(Snake&& s);
  Snake& operator=(const Snake& s);
  Snake& operator=(Snake&& s);
  ~Snake();

  void Update();

  void GrowBody();
  bool SnakeCell(int x, int y);
  void EstimateDirection();
  bool SnakeNeighbor(int x, int y);

  Direction direction = Direction::kUp;

  float speed{0.1f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;
  std::map<std::string, std::string> key_map;
  Type type;
  int id;
  std::vector<std::vector<CellType>> *domain_ptr;
  SDL_Point *goal_ptr; 
  int score=0;

 private:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  bool growing{false};
  int grid_width;
  int grid_height;
};

#endif
