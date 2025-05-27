#include "game.hpp"
#include <iostream>
#include <vector>

int main() {

    initDatabase(); // Initialiser database

    std::cout << "Welcome to the game!" << "\n";
    std::cout << "You have the following options: " << "\n";
    std::cout << "1. Create a new hero" << "\n";
    std::cout << "2. Load an existing hero" << "\n";
    std::cout << "3. Analyse" << "\n";
    std::cout << "4. Exit" << "\n";

    int choice;
    while (true) {
        std::cout << "Enter the number of your choice: ";
        std::cin >> choice;
        if (choice >= 1 && choice <= 4) break; // Valid choice
        std::cout << "Invalid choice. Please try again." << "\n";
    }
    if (choice == 4) {
        std::cout << "Exiting the game." << "\n";
        return 0;
    }

    std::cin.ignore(); // Clear the newline character from the input buffer
    std::string heroName;
    Hero hero("Default", 10, 2, 0, 1, 0, 10); // Default hero

    if (choice == 1) {
        std::cout << "Enter the name of your hero: ";
        std::getline(std::cin, heroName);
        hero = Hero(heroName, 10, 2, 0, 1, 0, 10); // Create a new hero
        saveHeroToDatabase(hero); // Save the new hero to a file
        std::cout << "Hero created and saved!" << "\n";
    } else if (choice == 2) {
        std::cout << "Enter the name of the hero you want to load: ";
        std::getline(std::cin, heroName);

        if (!heroExistsInDatabase(heroName)) {
            hero = Hero(heroName, 10, 2, 0, 1, 0, 10);
            saveHeroToDatabase(hero);
            std::cout << "Hero not found in database. A new hero has been created with the name: " << heroName << "\n";
        } else {
            std::cout << "Loading hero...\n";
            hero = loadHeroFromDatabase(heroName);
        }

        hero.resetHp();
        std::cout << "Hero loaded!" << "\n";
        std::cout << "Hero Name: " << hero.getName() << "\n";
        std::cout << "Hero Level: " << hero.getLevel() << "\n";
        std::cout << "Hero Health: " << hero.getHealth() << "/" << hero.getMaxHp() << "\n";
        std::cout << "Hero Strength: " << hero.getStrength() << "\n"; 
        std::cout << "Hero XP: " << hero.getXp() << "\n";
        std::cout << "Hero Gold: " << hero.getGold() << "\n";
        Weapon w = hero.getWeapon();
        std::cout << "Weapon equipped:\n";
        std::cout << "Name: " << w.getName() 
          << " | Damage: " << w.getDamage()
          << " | Strength Modifier: " << w.getStrengthModifier()
          << " | Durability: " << w.getDurability() << "\n";

    } else if (choice == 3) {
        
        int analysisChoice;
        while (true) {
            std::cout << "Chose what to analyse\n";
            std::cout << "1. Show heroes alphabetically\n";
            std::cout << "2. Show total kills by each hero\n";
            std::cout << "3. Show top hero for each weapon\n";
            std::cout << "4. Show kills per weapon for a specific hero\n";
            std::cout << "5. Exit\n";

            std::cout << "Enter the number of your choice: ";
            std::cin >> analysisChoice;

            if (analysisChoice < 1 || analysisChoice > 5) {
                std::cout << "Invalid choice. Please try again." << "\n";
                continue; // Prompt again
            }
            
            if (analysisChoice == 5) {
                std::cout << "Exiting analysis." << "\n";
                return 0;
            }

            std::cin.ignore(); // Clear the newline character from the input buffer

            if (analysisChoice == 1) {
                showHereoesAlphabetically();

            } else if (analysisChoice == 2) {
                showTotalKills();

            } else if (analysisChoice == 3) {
                showTopHeroForWeapon();

            } else if (analysisChoice == 4) {
                std::cout << "Enter the name of the hero: ";
                std::getline(std::cin, heroName);
                showKillsPerWeapon(heroName);
            }
            std::cout << "\n"; // Newline for better readability
        }
    }

    std::cout << "Good luck on your adventure " << hero.getName() << "\n";
    
    gameLoop(hero); // Start the game loop
    
    std::cout << "Thanks for playing!" << "\n";
    saveHeroToDatabase(hero); // Save hero
    return 0;
}

