#include "game.hpp"
#include <iostream>
#include <vector>

int main() {
    std::cout << "Welcome to the game!" << "\n";
    std::cout << "Enter your hero's name: ";
    std::string heroName;
    std::cin >> heroName;

    Hero hero(heroName, 10, 2, 0, 1); // Create a hero with default stats
    gameLoop(hero); // Start the game loop
    
    std::cout << "Thanks for playing!" << "\n";
    return 0;
}