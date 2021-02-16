#ifndef GAME_H
#define GAME_H

#include <random>
#include <vector>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

enum class CellType{empty=0, blocked=1, snake=2, food=3};

class Game {
 public:

  Game(std::size_t grid_width, std::size_t grid_height, std::size_t nsnakes, std::size_t nsnakes_human, std::vector<std::map<std::string,std::string>>& key_maps);
  Game(std::vector<std::vector<CellType>>&& domain_matrix, std::size_t nsnakes, std::size_t nsnakes_human, std::vector<std::map<std::string,std::string>>& key_maps);
  Game(const Game& g);
  Game(Game&& g);
  Game& operator=(const Game& g);
  Game& operator=(Game&& g);
  void UpdateDomainMatrix(void);
  void CheckBounds(int x, int y, int type);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  std::vector<int> GetScores() const;

 private:
  std::vector<Snake> snakes;
  SDL_Point food;
  std::vector<std::vector<CellType>> domain_matrix;
  std::vector<SDL_Point> blocked_cells;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  std::vector<int> scores;

  void InitializeBlockedCells(void);
  bool BlockedCell(int x, int y);
  void PlaceSnakes();
  void PlaceFood();
  void Update();
};

#endif
