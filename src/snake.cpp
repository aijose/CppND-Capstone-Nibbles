#include "snake.h"
#include "game.h"
#include <cmath>
#include <iostream>

Snake::Snake(const Snake &s): 
    direction{s.direction}, 
    speed{s.speed},
    size{s.size},
    alive{s.alive},
    head_x{s.head_x},
    head_y{s.head_y},
    body{s.body},
    key_map{s.key_map},
    type{s.type},
    id{s.id},
    domain_ptr{s.domain_ptr},
    goal_ptr{s.goal_ptr},
    score{s.score},
    grid_width{s.grid_width},
    grid_height{s.grid_height},
    growing{s.growing}
{
}

Snake::Snake(Snake &&s): 
    direction{s.direction}, 
    speed{s.speed},
    size{s.size},
    alive{s.alive},
    head_x{s.head_x},
    head_y{s.head_y},
    body{std::move(s.body)},
    key_map{std::move(s.key_map)},
    type{s.type},
    id{s.id},
    domain_ptr{s.domain_ptr},
    goal_ptr{s.goal_ptr},
    score{s.score},
    grid_width{s.grid_width},
    grid_height{s.grid_height},
    growing{s.growing}
{
}

Snake& Snake::operator=(const Snake &s)
{
    if (this == &s) return *this;
    direction = s.direction; 
    speed = s.speed;
    size = s.size;
    alive = s.alive;
    head_x = s.head_x;
    head_y = s.head_y;
    body = s.body;
    key_map = s.key_map;
    type = s.type;
    id = s.id;
    domain_ptr = s.domain_ptr;
    goal_ptr = s.goal_ptr;
    score = s.score;
    grid_width = s.grid_width;
    grid_height = s.grid_height;
    growing = s.growing;

    return *this;
}

Snake& Snake::operator=(Snake &&s)
{
    direction = s.direction; 
    speed = s.speed;
    size = s.size;
    alive = s.alive;
    head_x = s.head_x;
    head_y = s.head_y;
    body = std::move(s.body);
    key_map = std::move(s.key_map);
    type = s.type;
    id = s.id;
    domain_ptr = s.domain_ptr;
    goal_ptr = s.goal_ptr;
    score = s.score;
    grid_width = s.grid_width;
    grid_height = s.grid_height;
    growing = s.growing;

    return *this;
}

Snake::~Snake() {}

void Snake::Update() {
  if (type == Snake::Type::machine)
      DecideDirection();
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(
          head_y)};  // We first capture the head's cell before updating.
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // Capture the head's cell after updating.

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}

// This method identifies whether a cell within a certain distance of a snake cell
bool Snake::SnakeNeighbor(int x, int y, int distance) {
    return false;
    for (int i=-distance; i < distance+1; i++)  
        for (int j=-distance; j < distance+1; j++) {
            if (i == 0 && j == 0) continue;
            int xp = (x + i) % grid_width;
            int yp = (y + j) % grid_height;
            if ((*domain_ptr)[xp][yp] == CellType::snake) return true;
        }

    return false;
}

// This function determines whether the snake needs to change direction
// to reach its goal (food)
void Snake::DecideDirection(void) {
    SDL_Point current_cell{static_cast<int>(head_x), static_cast<int>(head_y)};

    long int best_score = -1000000;
    float penalty = 1; // Factor for penalizing being near a snake cell
    Direction best_dir = direction;
    for(auto dir: {Direction::kUp, Direction::kDown, Direction::kLeft, Direction::kRight}) {
        if (dir == Direction::kUp) {// Up
            int x = fmod(current_cell.x + grid_width, grid_width);
            int y = fmod(current_cell.y - 1 + grid_height, grid_height);
            long int score = 0;
            if ((*domain_ptr)[x][y] == CellType::blocked)
                score = -20*(grid_width + grid_height);
            else if ((*domain_ptr)[x][y] == CellType::snake)
                score = -10*(grid_width + grid_height);
            else {
                score = -(abs(goal_ptr->x - x) + abs(goal_ptr->y - y));
                if (SnakeNeighbor(x, y, 2)) score -= static_cast<int>(penalty*(grid_width + grid_height));
            }
            if (score > best_score) {
                best_score = score;
                best_dir = dir;
            }
        }
        else if (dir == Direction::kDown) {// Down
            int x = fmod(current_cell.x + grid_width, grid_width);
            int y = fmod(current_cell.y + 1 + grid_height, grid_height);
            long int score = 0;
            if ((*domain_ptr)[x][y] == CellType::blocked)
                score = -20*(grid_width + grid_height);
            else if ((*domain_ptr)[x][y] == CellType::snake)
                score = -10*(grid_width + grid_height);
            else {
                score = -(abs(goal_ptr->x - x) + abs(goal_ptr->y - y));
                if (SnakeNeighbor(x, y, 2)) score -= static_cast<int>(penalty*(grid_width + grid_height));
            }
            if (score > best_score) {
                best_score = score;
                best_dir = dir;
            }
        }
        else if (dir == Direction::kLeft) {// Left
            int x = fmod(current_cell.x - 1 + grid_width, grid_width);
            int y = fmod(current_cell.y + grid_height, grid_height);
            long int score = 0;
            if ((*domain_ptr)[x][y] == CellType::blocked)
                score = -20*(grid_width + grid_height);
            else if ((*domain_ptr)[x][y] == CellType::snake)
                score = -10*(grid_width + grid_height);
            else {
                score = -(abs(goal_ptr->x - x) + abs(goal_ptr->y - y));
                if (SnakeNeighbor(x, y, 2)) score -= static_cast<int>(penalty*(grid_width + grid_height));
            }
            if (score > best_score) {
                best_score = score;
                best_dir = dir;
            }
        }
        else if (dir == Direction::kRight) {// Right
            int x = fmod(current_cell.x + 1 + grid_width, grid_width);
            int y = fmod(current_cell.y + grid_height, grid_height);
            long int score = 0;
            if ((*domain_ptr)[x][y] == CellType::blocked)
                score = -20*(grid_width + grid_height);
            else if ((*domain_ptr)[x][y] == CellType::snake)
                score = -10*(grid_width + grid_height);
            else {
                score = -(abs(goal_ptr->x - x) + abs(goal_ptr->y - y));
                if (SnakeNeighbor(x, y, 2)) score -= static_cast<int>(penalty*(grid_width + grid_height));
            }
            if (score > best_score) {
                best_score = score;
                best_dir = dir;
            }
        }
    }
    if (best_dir == direction) return;
    direction = best_dir;
}

void Snake::UpdateHead() {
  switch (direction) {
    case Direction::kUp:
      head_y -= speed;
      break;

    case Direction::kDown:
      head_y += speed;
      break;

    case Direction::kLeft:
      head_x -= speed;
      break;

    case Direction::kRight:
      head_x += speed;
      break;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  // Add previous head location to vector
  body.push_back(prev_head_cell);

  if (!growing) {
    // Remove the tail from the vector.
    body.erase(body.begin());
  } else {
    growing = false;
    size++;
  }

  // Check if the snake has died.
  for (auto const &item : body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      alive = false;
      std::cout << std::endl << "Player " << id+1 << " has collided with himself!" << std::endl;
    }
  }
}

void Snake::GrowBody() { growing = true; score++; }

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y) {
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
    return true;
  }
  for (auto const &item : body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}
