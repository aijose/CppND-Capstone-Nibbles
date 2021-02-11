#ifndef GAME_H
#define GAME_H

#include <random>
#include <vector>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  Game(std::vector<std::vector<int>>&& domain_matrix);
  Game(const Game& g);
  Game(Game&& g);
  Game& operator=(const Game& g);
  Game& operator=(Game&& g);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;

 private:
  Snake snake;
  SDL_Point food;
  std::vector<std::vector<int>> domain_matrix;
  std::vector<SDL_Point> blocked_cells;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};

  void InitializeBlockedCells(void);
  bool BlockedCell(int x, int y);
  void PlaceSnake();
  void PlaceFood();
  void Update();
};

#endif
