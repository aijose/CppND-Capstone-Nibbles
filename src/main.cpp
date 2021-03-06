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
  std::string config_file = "";
  std::vector<std::vector<CellType>> domain_matrix;
  int grid_width = kGridWidth;
  int grid_height = kGridHeight;
  bool layout_file_flag = false;
  bool config_file_flag = false;
  if (argc > 1) {
      // Read the custom layout file
      for (int i = 1; i < argc; ++i) {
          if (std::string_view{argv[i]} == "-l" && ++i < argc) {
              domain_file = argv[i];
              layout_file_flag = true;
          }
          if (std::string_view{argv[i]} == "-c" && ++i < argc) {
              config_file = argv[i];
              config_file_flag = true;
          }
      }
  }
  if (layout_file_flag) {
      std::ifstream is{domain_file};
      is >> grid_width >> grid_height;
      domain_matrix = std::move(std::vector<std::vector<CellType>>(grid_height, std::vector<CellType>(grid_width, CellType::empty)));
      int count  = 0;
      for (int i=0; i < grid_height; i++)
          for (int j=0; j < grid_width; j++) {
              int cell_type;
              is >> cell_type;
              if (cell_type != 0)
                  domain_matrix[i][j] = CellType::blocked;
              if (domain_matrix[i][j] == CellType::blocked) count++;
          }
  }
  else {
      domain_matrix = std::move(std::vector<std::vector<CellType>>(grid_height, std::vector<CellType>(grid_width, CellType::empty)));
  }

  int nsnakes, nsnakes_human, nsnakes_computer;
  std::vector<std::map<std::string,std::string>> key_maps;
  std::map<std::string,std::string> single_key_map;

  if (config_file_flag) {
      std::ifstream is{config_file};
      is >> nsnakes;
      is >> nsnakes_human;
      for(int i=0; i < nsnakes_human; i++) {
          is >> single_key_map["up"] >>  single_key_map["down"] >>  single_key_map["left"] >>  single_key_map["right"];
          key_maps.push_back(single_key_map);
      }
  } else { // Use default configuration of 3 computer players and 1 human player
      nsnakes = 2;
      nsnakes_human = 1;
      key_maps = {
          {{"up", "up"}, {"down", "down"}, {"left", "left"}, {"right", "right"}}, 
          {{"up", "w"}, {"down", "s"}, {"left", "a"}, {"right", "d"}},
          {{"up", "k"}, {"down", "j"}, {"left", "h"}, {"right", "l"}},
          {{"up", "g"}, {"down", "b"}, {"left", "v"}, {"right", "n"}}
      };
  }
  //std::cout << "Enter the total number of snakes: ";
  //std::cin >> nsnakes;
  //std::cout << "Enter the number of human players: ";
  //std::cin >> nsnakes_human;
  //for(int i=0; i < nsnakes_human; i++) {
  //    std::cout << "Enter the up, down, left, right keys for Player#" << i+1 << ": ";
  //    std::cin >> single_key_map["up"] >>  single_key_map["down"] >>  single_key_map["left"] >>  single_key_map["right"];
  //    key_maps.push_back(single_key_map);
  //}

  if (nsnakes > 4) {
      std::cout << "The maximum number of snakes allowed is 4!" << std::endl;
      return 0;
  }

  Renderer renderer(kScreenWidth, kScreenHeight, grid_width, grid_height);
  Controller controller;
  Game game(grid_width, grid_height, nsnakes, nsnakes_human, key_maps);

  if (domain_file != "") game = std::move(Game(std::move(domain_matrix), nsnakes, nsnakes_human, key_maps));

  game.Run(controller, renderer, kMsPerFrame);

  std::cout << "Game has terminated successfully!\n";
  std::vector<int> scores = game.GetScores();
  std::cout << std::endl << "FINAL SCORES:" << std::endl;
  for(int i=0; i < scores.size(); i++) {
      std::cout << "Player#" << i+1 << " score: " << scores[i] << std::endl;
  }

  return 0;
}
