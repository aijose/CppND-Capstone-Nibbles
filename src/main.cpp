#include <iostream>
#include <string>
#include <fstream>
#include "controller.h"
#include "game.h"
#include "renderer.h"

int main(int argc, char** argv) {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  std::string domain_file = "";
  std::vector<std::vector<int>> domain_matrix;
  int grid_width = kGridWidth;
  int grid_height = kGridHeight;
  if (argc > 1) {
      for (int i = 1; i < argc; ++i)
          if (std::string_view{argv[i]} == "-d" && ++i < argc)
              domain_file = argv[i];
      std::ifstream is{domain_file};
      is >> grid_width >> grid_height;
      domain_matrix = std::move(std::vector<std::vector<int>>(grid_height, std::vector<int>(grid_width, 0)));
      int count  = 0;
      for (int i=0; i < grid_height; i++)
          for (int j=0; j < grid_width; j++) {
              is >> domain_matrix[i][j];
              if (domain_matrix[i][j] == 1) count++;
          }
  }
  else {
      domain_matrix = std::move(std::vector<std::vector<int>>(grid_height, std::vector<int>(grid_width, 0)));
  }

  int nsnakes;
  //std::cout << "Enter the number of snakes: ";
  //std::cin >> nsnakes;
  //for(int i=0; i < nsnakes; i++) {
  //    std::cout << "Enter the up, down, left, right keys for snake #" << i;
  //    std::map<std::string, std::string> single_key_map;
  //    std::cin >> single_key_map["up"] >>  single_key_map["down"] >>  single_key_map["left"] >>  single_key_map["right"];
  //}
  nsnakes = 2;
  std::vector<std::map<std::string,std::string>> key_maps = {
      {{"up", "up"}, {"down", "down"}, {"left", "left"}, {"right", "right"}}, 
      {{"up", "w"}, {"down", "s"}, {"left", "a"}, {"right", "d"}}
  };

  Renderer renderer(kScreenWidth, kScreenHeight, grid_width, grid_height);
  Controller controller;
  Game game(grid_width, grid_height, nsnakes, key_maps);

  if (domain_file != "") game = std::move(Game(std::move(domain_matrix), nsnakes, key_maps));

  game.Run(controller, renderer, kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  return 0;
}
