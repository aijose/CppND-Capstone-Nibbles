#include "game.h"
#include <iostream>
#include <vector>
#include <string>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height, int nsnakes, std::map<std::string,std::string> key_maps)
    : snakes{2, Snake(grid_width, grid_height)},
      engine(dev()),
      random_w(0, static_cast<int>(grid_width)),
      random_h(0, static_cast<int>(grid_height)),
      domain_matrix{grid_height, std::vector<int>(0, grid_width)} {
  for(int i=0; i < nsnakes; i++) {
      snake[i].key_map = key_maps[i];
  }
  InitializeBlockedCells();
  PlaceSnakes();
  PlaceFood();
}

Game::Game(std::vector<std::vector<int>>&& matrix, int nsnakes, std::map<std::string,std::string> key_maps)
    : snakes{2, Snake(matrix[0].size(), matrix.size())},
      engine(dev()),
      random_w(0, static_cast<int>(matrix[0].size())),
      random_h(0, static_cast<int>(matrix.size())),
      domain_matrix{matrix} {
  for(int i=0; i < nsnakes; i++) {
      snake[i].key_map = key_maps[i];
  }
  InitializeBlockedCells();
  PlaceSnake();
  PlaceFood();
}

Game::Game(const Game& g)
    : snakes{g.snakes}, 
      food{g.food}, 
      domain_matrix{g.domain_matrix}, 
      engine(dev()), 
      random_w(0, static_cast<int>(g.domain_matrix[0].size())), 
      random_h(0, static_cast<int>(g.domain_matrix.size()))
{
}

Game::Game(Game&& g)
    : snakes{g.snakes}, 
      food{g.food}, 
      domain_matrix{std::move(g.domain_matrix)}, 
      engine(dev()), 
      random_w(0, static_cast<int>(g.domain_matrix[0].size())), 
      random_h(0, static_cast<int>(g.domain_matrix.size()))
{
}

Game& Game::operator=(const Game& g)
{
  food = g.food;
  domain_matrix = g.domain_matrix;
  blocked_cells = g.blocked_cells;
  engine = g.engine;
  random_w = g.random_w;
  random_h = g.random_h;

  return *this;
}

Game& Game::operator=(Game&& g)
{
  food = g.food;
  domain_matrix = std::move(g.domain_matrix);
  blocked_cells = std::move(g.blocked_cells);
  engine = g.engine;
  random_w = g.random_w;
  random_h = g.random_h;

  return *this;
}

void Game::InitializeBlockedCells(void) {
  for (int i=0; i < domain_matrix.size(); i++)
      for (int j=0; j < domain_matrix[0].size(); j++) {
          if (domain_matrix[i][j] != 1) continue;
          SDL_Point bc;
          bc.x = j;
          bc.y = i;
          blocked_cells.push_back(bc);
      }

}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snakes);
    Update();
    renderer.Render(snakes, blocked_cells, food);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceSnakes() {
    for(auto& snake: snakes) {
        int x, y;
        bool exit_flag = true;
        while (exit_flag) {
            x = random_w(engine);
            y = random_h(engine);
            // Check that the location is not occupied by a snake item before placing
            // food.
            if (!BlockedCell(x, y)) {
                snake.head_x = x;
                snake.head_y = y;
                exit_flag = false;
            }
        }
    }
}

bool Game::BlockedCell(int x, int y) {
  for (auto const &item : blocked_cells) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);

    // Check that the location is not occupied by a snake item or a blocked
    // cell before placing food.
    for(auto& snake: snakes) {
        if (!snake.SnakeCell(x, y)) {
            food.x = x;
            food.y = y;
            return;
        }
    }
  }
}

void Game::Update() {
    for(auto& snake: snakes) {
        if (!snake.alive) return;
    }

  snake.Update();
  // Check if the snake has collided with a blocked cell
  for (auto const &item : blocked_cells) {
    if (abs(snake.head_x - item.x) < 0.5 && abs(snake.head_y - item.y) < 0.5) {
      snake.alive = false;
      break;
    }
  }

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }
