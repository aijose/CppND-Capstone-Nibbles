#include "controller.h"
#include <iostream>
#include "SDL.h"
#include "snake.h"

std::map<std::string,Uint8> sdl_map = {
    {"up", SDLK_UP},
    {"down", SDLK_DOWN},
    {"left", SDLK_LEFT},
    {"right", SDLK_RIGHT},
    {"a", SDLK_a},
    {"b", SDLK_b},
    {"c", SDLK_c},
    {"d", SDLK_d},
    {"e", SDLK_e},
    {"f", SDLK_f},
    {"g", SDLK_g},
    {"h", SDLK_h},
    {"i", SDLK_i},
    {"j", SDLK_j},
    {"k", SDLK_k},
    {"l", SDLK_l},
    {"m", SDLK_m},
    {"n", SDLK_n},
    {"o", SDLK_o},
    {"p", SDLK_p},
    {"q", SDLK_q},
    {"r", SDLK_r},
    {"s", SDLK_s},
    {"t", SDLK_t},
    {"u", SDLK_u},
    {"v", SDLK_v},
    {"w", SDLK_w},
    {"x", SDLK_x},
    {"y", SDLK_y},
    {"z", SDLK_z},
    {"0", SDLK_0},
    {"1", SDLK_1},
    {"2", SDLK_2},
    {"3", SDLK_3},
    {"4", SDLK_4},
    {"5", SDLK_5},
    {"6", SDLK_6},
    {"7", SDLK_7},
    {"8", SDLK_8},
    {"9", SDLK_9},
    {":", SDLK_9},
    {";", SDLK_9},
    {"<", SDLK_9},
    {"=", SDLK_9},
    {">", SDLK_9},
    {"?", SDLK_9},
    {"@", SDLK_9},
    {"[", SDLK_9},
    {"\\", SDLK_9},
    {"]", SDLK_9},
    {"^", SDLK_9},
    {"_", SDLK_9},
    {"", SDLK_SPACE},
    {"!", SDLK_EXCLAIM},
    {"\"", SDLK_QUOTEDBL},
    {"#", SDLK_HASH},
    {"$", SDLK_DOLLAR},
    {"&", SDLK_AMPERSAND},
    {"'", SDLK_QUOTE}
};

void Controller::ChangeDirection(Snake &snake, Snake::Direction input,
                                 Snake::Direction opposite) const {
  if (snake.direction != opposite || snake.size == 1) snake.direction = input;
  return;
}

void Controller::HandleInput(bool &running, std::vector<Snake> &snakes) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_KEYDOWN) {
      for(auto& snake: snakes) {
          if (snake.type == Snake::Type::machine) continue;
          if (static_cast<Uint8>(e.key.keysym.sym) == sdl_map[snake.key_map["up"]])
              ChangeDirection(snake, Snake::Direction::kUp, Snake::Direction::kDown);
          else if (static_cast<Uint8>(e.key.keysym.sym) == sdl_map[snake.key_map["down"]])
              ChangeDirection(snake, Snake::Direction::kDown, Snake::Direction::kUp);
          else if (static_cast<Uint8>(e.key.keysym.sym) == sdl_map[snake.key_map["left"]])
              ChangeDirection(snake, Snake::Direction::kLeft, Snake::Direction::kRight);
          else if (static_cast<Uint8>(e.key.keysym.sym) == sdl_map[snake.key_map["right"]])
              ChangeDirection(snake, Snake::Direction::kRight, Snake::Direction::kLeft);
      }
    }
  }
}
