#include "game.hpp"
#include <iostream>
#include <vector>

int main() {

    std::vector<Hero> heroes = getHeroes();

    std::cout << "Welcome to the game!" << "\n";
    std::cout << "Chose your hero: " << "\n";

    for (size_t i = 0; i < heroes.size(); ++i) {
        std::cout << i + 1 << ". " << heroes[i].getName() << "\n";
    }
    std::cout << "4. Exit" << "\n";


    int heroChoice;
    while (true) {
        std::cout << "Enter the number of your choice: ";
        std::cin >> heroChoice;
        if (heroChoice >= 1 && heroChoice <= 4) break; // Valid choice
        std::cout << "Invalid choice. Please try again." << "\n";
    }

    if (heroChoice == 4) {
        std::cout << "Exiting the game." << "\n";
        return 0;
    } 

    Hero hero = heroes[heroChoice - 1]; // Get the selected hero

    std::cout << "You chose: " << hero.getName() << "\n";
    
    gameLoop(hero); // Start the game loop
    
    std::cout << "Thanks for playing!" << "\n";
    return 0;
}