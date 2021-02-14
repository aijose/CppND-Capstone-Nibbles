#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"

//std::map<std::string,Uint8> sdl_map = {
//    {"a", SDLK_a},
//    {"b", SDLK_b},
//    {"c", SDLK_c},
//    {"d", SDLK_d},
//    {"e", SDLK_e},
//    {"f", SDLK_f},
//    {"g", SDLK_g},
//    {"h", SDLK_h},
//    {"i", SDLK_i},
//    {"j", SDLK_j},
//    {"k", SDLK_k},
//    {"l", SDLK_l},
//    {"m", SDLK_m},
//    {"n", SDLK_n},
//    {"o", SDLK_o},
//    {"p", SDLK_p},
//    {"q", SDLK_q},
//    {"r", SDLK_r},
//    {"s", SDLK_s},
//    {"t", SDLK_t},
//    {"u", SDLK_u},
//    {"v", SDLK_v},
//    {"w", SDLK_w},
//    {"x", SDLK_x},
//    {"y", SDLK_y},
//    {"z", SDLK_z},
//    {"0", SDLK_0},
//    {"1", SDLK_1},
//    {"2", SDLK_2},
//    {"3", SDLK_3},
//    {"4", SDLK_4},
//    {"5", SDLK_5},
//    {"6", SDLK_6},
//    {"7", SDLK_7},
//    {"8", SDLK_8},
//    {"9", SDLK_9},
//    {":", SDLK_9},
//    {";", SDLK_9},
//    {"<", SDLK_9},
//    {"=", SDLK_9},
//    {">", SDLK_9},
//    {"?", SDLK_9},
//    {"@", SDLK_9},
//    {"[", SDLK_9},
//    {"\\", SDLK_9},
//    {"]", SDLK_9},
//    {"^", SDLK_9},
//    {"_", SDLK_9},
//    {"", SDLK_SPACE},
//    {"!", SDLK_EXCLAIM},
//    {"\"", SDLK_QUOTEDBL},
//    {"#", SDLK_HASH},
//    {"$", SDLK_DOLLAR},
//    {"&", SDLK_AMPERSAND},
//    {"'", SDLK_QUOTE}
//};

class Controller {
 public:
  void HandleInput(bool &running, std::vector<Snake> &snake) const;

 private:
  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;
};

#endif
