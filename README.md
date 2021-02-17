# CPPND: Capstone Snake Game Example

This is a starter repo for the Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). The code for this repo was inspired by [this](https://codereview.stackexchange.com/questions/212296/snake-game-in-c-with-sdl) excellent StackOverflow post and set of responses.

<img src="snake_game.gif"/>

The Capstone Project gives you a chance to integrate what you've learned throughout this program. This project will become an important part of your portfolio to share with current and future colleagues and employers.

In this project, you can build your own C++ application or extend this Snake game, following the principles you have learned throughout this Nanodegree Program. This project will demonstrate that you can independently create applications using a wide range of C++ features.

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  * Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source.
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`. 
5. Optionally, a layout file can be used to specify different layouts for the
   game. Sample layouts can be found in the data/ folder. The layout file can
   be specified by using the "-l" command line option, as shown below:

   ./SnakeGame -l ../data/horizontal\_divider.txt

## Project Description

This project extends the baseline snake game that was provided. In the baseline
snake game, the user controls a snake. The snake moves around the domain trying
to eat food which pops up 

The following extensions have been made:

* The baseline snake game has only one layout, where the entire domain is
  empty. The extended snake game allows arbitrary layouts that can be specified
  using a layout file. A layout file specifies blocked cells (walls) in the
  game domain. Sample domains are provided in the data/ folder.

* The baseline snake game involves only one player. The extended snake game
  allows multiple players.

* The baseline snake game involves only human players. The extended snake game
  allows both human and machine players. A maximum of four players (machine and
  human) are allowed. The machine players are programmed to seek the food
  automatically and have been found to be better than most human players.

## File and Class Structure

The source code can be found in the ./src folder. The layout files can be found
in the ./data folder.

The key classes in the code are listed below:

* Snake: This class data and methods that model the properties and behavior of
  a single snake. The agent behind a snake can be a human or a computer.

* Game: This class brings together all the elements of the game, such as the
  snakes, layout, food, etc. There is only one instance of this class.

* Controller: The controller handles input from the player(s) and controls the
  snake(s) movement accordingly.

* Renderer: This class is responsible for displaying the different components
  of the game at each frame of the game. Besides the snakes, it is responsible
  for displaying the walls (blocked cells), food and scores.

## Addressing Rubric Points

### Addressing Rubric Points

* A README with instructions is included with the project

* The README indicates which project is chosen.

* The README includes information about each rubric point addressed.

### Compiling and Testing

* The submission must compile and run.

### Loops, Functions, I/O

* The project demonstrates an understanding of C++ functions and control structures.
  * Different C++ control structures such as if-else, for loops, switch, etc. are used

* The project reads data from a file and process the data, or the program writes data to a file.
  * The program reads a text file and uses it to define the layout for the game

* The project accepts user input and processes the input.
  * The program accepts the user inputs for controlling the snakes. The user inputs can be customized for multiple players.

### Object Oriented Programming

* The project uses Object Oriented Programming techniques.
  * The project uses multiple classses (*Controller*, *Game*, *Renderer*, *Snake*)

* Classes use appropriate access specifiers for class members.
  * Access specifiers have been used in all classes to control access to class members for all classes

* Class constructors utilize member initialization lists.
  * Member initialization lists have been used in all the constructors of the *Game* class.

### Memory Management

* The project makes use of references in function declarations.
  * References are used in the following functions - *Snake::UpdateBody, Renderer::Render, Game::Run, Controller::HandleInput, Controller::ChangeDirection*

* The project uses scope / Resource Acquisition Is Initialization (RAII) where appropriate.

* The project follows the Rule of 5.

* The project uses move semantics to move data, instead of copying it, where possible.
