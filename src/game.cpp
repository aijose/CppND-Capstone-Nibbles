#include "game.h"
#include <iostream>
#include <vector>
#include <string>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height, std::size_t nsnakes, std::size_t nsnakes_human, std::vector<std::map<std::string,std::string>>& key_maps)
    : snakes{nsnakes, Snake(grid_width, grid_height)},
      engine(dev()),
      scores{static_cast<int>(nsnakes), 0},
      random_w(0, static_cast<int>(grid_width)-1),
      random_h(0, static_cast<int>(grid_height)-1),
      domain_matrix{grid_height, std::vector<CellType>(grid_width, CellType::empty)} {
  for(int i=0; i < nsnakes_human; i++) {
      snakes[i].key_map = key_maps[i];
      snakes[i].type = Snake::Type::human;
      snakes[i].id = i;
  }
  InitializeBlockedCells();
  PlaceSnakes();
  PlaceFood();
  UpdateDomainMatrix();
  for(int i=nsnakes_human; i < nsnakes; i++) {
      snakes[i].type = Snake::Type::machine;
      snakes[i].id = i;
      snakes[i].domain_ptr = &domain_matrix;
      snakes[i].goal_ptr = &food;
  }
}

Game::Game(std::vector<std::vector<CellType>>&& matrix, std::size_t nsnakes, std::size_t nsnakes_human, std::vector<std::map<std::string,std::string>>& key_maps)
    : snakes{nsnakes, Snake(matrix[0].size(), matrix.size())},
      engine(dev()),
      scores{static_cast<int>(nsnakes), 0},
      random_w(0, static_cast<int>(matrix[0].size())-1),
      random_h(0, static_cast<int>(matrix.size())-1),
      domain_matrix{matrix} {
  for(int i=0; i < nsnakes_human; i++) {
      snakes[i].key_map = key_maps[i];
      snakes[i].type = Snake::Type::human;
      snakes[i].id = i;
  }
  InitializeBlockedCells();
  PlaceSnakes();
  PlaceFood();
  UpdateDomainMatrix();
  for(int i=nsnakes_human; i < nsnakes; i++) {
      snakes[i].type = Snake::Type::machine;
      snakes[i].id = i;
      snakes[i].domain_ptr = &domain_matrix;
      snakes[i].goal_ptr = &food;
  }
}

Game::Game(const Game& g)
    : snakes{g.snakes}, 
      food{g.food}, 
      domain_matrix{g.domain_matrix}, 
      engine(dev()), 
      random_w(0, static_cast<int>(g.domain_matrix[0].size())-1), 
      random_h(0, static_cast<int>(g.domain_matrix.size())-1)
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
          if (domain_matrix[i][j] != CellType::blocked) continue;
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
      renderer.UpdateWindowTitle(GetScores(), frame_count);
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
    if(x == food.x && y == food.y) continue;

    // Check that the location is not occupied by a snake item or a blocked
    // cell before placing food.
    if(BlockedCell(x, y)) continue;
    bool snake_flag = false;
    for(auto& snake: snakes)
        if (snake.SnakeCell(x, y)) snake_flag = true;
    if (!snake_flag) {
        food.x = x;
        food.y = y;
        return;
    }
  }
}

void Game::Update() {
  UpdateDomainMatrix();
  for(auto& snake: snakes)
      if (!snake.alive) return;

  for(auto& snake: snakes) {
      snake.Update();
      // Check if the snake has collided with a blocked cell
      for (auto const &item : blocked_cells) {
        if (static_cast<int>(snake.head_x) == item.x && static_cast<int>(snake.head_y) == item.y) {
          snake.alive = false;
          break;
        }
      }

      int new_x = static_cast<int>(snake.head_x);
      int new_y = static_cast<int>(snake.head_y);

      // Check if there's food over here
      if (food.x == new_x && food.y == new_y) {
        snake.score++;
        PlaceFood();
        // Grow snake and increase speed.
        snake.GrowBody();
        snake.speed += 0.02;
      }
  }
  // Check if the snakes have collided with each other
  for(int i=0; i < snakes.size(); i++) {
      // Check if the snake has collided with another snake
      for(int j=0; j < snakes.size(); j++) {
          if (i == j) continue;
          for (auto const &item : snakes[j].body) {
              if (static_cast<int>(snakes[i].head_x) == item.x && static_cast<int>(snakes[i].head_y) == item.y) {
                  snakes[i].alive = false;
                  goto outside_loops;
              }
          }
      }
  }
outside_loops: ;
  UpdateDomainMatrix();
}

void Game::CheckBounds(int x, int y, int type) {
    if(x >= 0 && x < domain_matrix[0].size() && y >= 0 && y < domain_matrix.size()) {
        return;
    }
    else {
        std::cout << "Type = " << type << std::endl;
        std::cout << std::endl << "x = " << x << ", y = " << y << std::endl;
        std::cout << std::endl << "Xlimit = " << domain_matrix[0].size() << ", Ylimit = " << domain_matrix.size() << std::endl;
    }
}

void Game::UpdateDomainMatrix(void) {
  for(int i = 0; i < domain_matrix.size(); i++)
      for(int j = 0; j < domain_matrix[i].size(); j++)
          domain_matrix[i][j] = CellType::empty;
  for (auto const &item : blocked_cells)
      domain_matrix[item.x][item.y] = CellType::blocked; 
  for (auto const& snake: snakes) {
      domain_matrix[static_cast<int>(snake.head_x)][static_cast<int>(snake.head_y)] = CellType::snake;
      for (auto const &item : snake.body)
          domain_matrix[item.x][item.y] = CellType::snake; 
  }
  domain_matrix[food.x][food.y] = CellType::food;
}

std::vector<int> Game::GetScores() const { 
    std::vector<int> scores(snakes.size(), 0);
    for(int i=0; i < snakes.size(); i++)
        scores[i] = snakes[i].score;
    return scores; 
}
