#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::Render(std::vector<Snake>& snakes, std::vector<SDL_Point>& blocked_cells, SDL_Point const &food) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;
  std::vector<std::vector<Uint8>> body_color = {
      {0xCC,0x99,0xFF}, //light purple
      {0xE5,0xFF,0xCC}, //light green
      {0xFF,0x99,0xFF}, //light magenta
      {0x99,0xCC,0xFF}  //light blue
  };
  std::vector<std::vector<Uint8>> head_color = {
      {0x7F,0x00,0xFF}, //white smoke
      {0x00,0xFF,0x00}, //green
      {0xFF,0x00,0xFF}, //magenta
      {0x00,0x00,0xFF}  //blue
  };

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render food
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  SDL_RenderFillRect(sdl_renderer, &block);

  // Render blocked cells
  for(SDL_Point const &point : blocked_cells) {
      SDL_SetRenderDrawColor(sdl_renderer, 0xC0, 0xC0, 0xC0, 0xFF);
      block.x = point.x * block.w;
      block.y = point.y * block.h;
      SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render snake's body
  int count = -1;
  bool dead_snake = false;
  for(auto& snake: snakes) {
      count++;
      SDL_SetRenderDrawColor(sdl_renderer, body_color[count][0], body_color[count][1], body_color[count][2], 0xFF);
      for (SDL_Point const &point : snake.body) {
          block.x = point.x * block.w;
          block.y = point.y * block.h;
          SDL_RenderFillRect(sdl_renderer, &block);
      }

      // Render snake's head
      block.x = static_cast<int>(snake.head_x) * block.w;
      block.y = static_cast<int>(snake.head_y) * block.h;
      if (snake.alive) {
          SDL_SetRenderDrawColor(sdl_renderer, head_color[count][0], head_color[count][1], head_color[count][2], 0xFF);
      } else {
          dead_snake = true;
      }
      SDL_RenderFillRect(sdl_renderer, &block);
  }
  // Color the dead snake's head red
  if (dead_snake)
      for(auto& snake: snakes) {
          // Render snake's head
          block.x = static_cast<int>(snake.head_x) * block.w;
          block.y = static_cast<int>(snake.head_y) * block.h;
          if (!snake.alive) {
              SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
              SDL_RenderFillRect(sdl_renderer, &block);
          }
      }

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(std::vector<int>&& scores, int fps) {
  std::string all_scores;
  for(int i=0; i < scores.size(); i++) {
      all_scores += "Player#" + std::to_string(i+1) + "=" + std::to_string(scores[i]) + ", ";
  }
  std::string title{all_scores + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
