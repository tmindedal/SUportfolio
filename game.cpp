#include "game.hpp"
#include <iostream>
#include <fstream>


// Implementation of the Character class
// Constructor
Character::Character(std::string name, int health, int strength) {
    this->name = name;
    hp = health;
    this->strength = strength;
}

// Defining the functions of the Character class.
std::string Character::getName() const { return name; }

int Character::getHealth() const { return hp; }

int Character::getStrength() const { return strength; }

void Character::damage(int amount) { hp -= amount; }

bool Character::isAlive() const { return hp > 0; }


// Implementation of the Hero class
Hero::Hero(std::string name, int health, int strength, int xp, int level, int gold) : Character(name, health, strength) {
    this->name = name;
    hp = health;
    this->strength = strength;
    this->level = level;
    this->xp = xp;
    maxHp = health; // Set max HP so we can reset the HP after every fight
    this->gold = gold; // Initialize gold
}

// Defining the functions of the Hero class.
int Hero::getXp() const { return xp; }

int Hero::getLevel() const { return level; }

void Hero::gainXp(int amount) { xp += amount; }

void Hero::levelUp() {
    if (xp >= level * 1000) {
        strength++;
        maxHp += 2;
        xp -= level * 1000; // Reset XP after leveling up
        level++;
        std::cout << name << " leveled up to level " << level << "\n";
        std::cout << "New strength: " << strength << "\n";
        std::cout << "New health: " << maxHp << "\n\n";
    }
}

void Hero::resetHp() { 
    this->hp = maxHp; // Reset HP to max after every fight
} 

Monster::Monster(std::string name, int health, int strength, int xpReward) : Character(name, health, strength) {
    this->name = name;
    this->hp = health;
    this->strength = strength;
    this->xpReward = xpReward;
}

int Monster::getXpReward() const { return xpReward; }


void combat(Hero &hero, Monster &monster) {
    std::cout << "The battle between " << hero.getName() << " and " << monster.getName() << " begins! " << "\n\n";

    while (hero.isAlive() && monster.isAlive()) {
        std::cout << hero.getName() << " attacks " << monster.getName() << " and deals " << hero.getStrength() << " damage!" << "\n";
        monster.damage(hero.getStrength());
        std::cout << monster.getName() << " has " << monster.getHealth() << " health left." << "\n";
        if (!monster.isAlive()) break;

        std::cout << monster.getName() << " attacks " << hero.getName() << " and deals " << monster.getStrength() << " damage!" << "\n";
        hero.damage(monster.getStrength());
        std::cout << hero.getName() << " has " << hero.getHealth() << " health left." << "\n\n";
    }

    if (hero.isAlive()) {
        std::cout << "\n" << "You win the battle " << hero.getName() << "!" << "\n\n";
        hero.gainXp(monster.getXpReward());
        std::cout << hero.getName() << " gained " << monster.getXpReward() << " XP!" << "\n\n";
        hero.levelUp(); // Check if the hero levels up after gaining XP

    } else {
        std::cout << "You lost the battle " << hero.getName() << "!" << "\n\n";
    }
}


std::vector<Monster> getMonsters() {
    return {
        Monster("Horse", 4, 1, 100),
        Monster("Weak Goblin", 4, 2, 200),
        Monster("Strong Goblin", 8, 3, 400),
        Monster("Stronger Goblin", 10, 4, 500),
        Monster("The Strongest Goblin", 15, 5, 800),
        Monster("The Monkey King", 30, 5, 1000),
        Monster("The Unicorn", 5, 8, 1500),
        Monster("The Dragon", 100, 10, 3000)
    };
}

std::vector<Hero> getHeroes() {
    return {
        Hero("Knight", 10, 2, 0, 1), // name, health, strength, xp, level
        Hero("Mage", 10, 2, 0, 1),
        Hero("Witch", 10, 2, 0, 1)
    };
}

void gameLoop(Hero& hero) {
    while (true) {
        std::cout << "Press 0 to exit, 1 to fight a monster or 2 to enter a cave: " << "\n";
        char choice;
        std::cin >> choice;

        if (choice == '0') break;
        
        else if (choice == '1') {
            std::vector<Monster> monsters = getMonsters();
            for (size_t i = 0; i < monsters.size(); ++i) {
                std::cout << i + 1 << ". " << monsters[i].getName() << " (Health: " << monsters[i].getHealth() << ", Strength: " << monsters[i].getStrength() << ")\n";
            }
            int monsterChoice;
            std::cout << "Choose a monster to fight (1-" << monsters.size() << "): ";
            std::cin >> monsterChoice;
            if (monsterChoice > 0 && monsterChoice <= static_cast<int>(monsters.size())) {
                combat(hero, monsters[monsterChoice - 1]);
                hero.resetHp(); // Reset hero's HP after the fight
            } else {
                std::cout << "Invalid choice." << "\n";
                continue; // Skip to the next iteration of the loop;
            }
        }

        else if (choice == '2') {
            std::vector<Cave> caves = generateCaves(hero.getLevel());
            for (size_t i = 0; i < caves.size(); ++i) {
                std::cout << i + 1 << ". " << caves[i].getName() << " (Gold Reward: " << caves[i].getGoldReward() << ")\n";
            }
            int caveChoice;
            std::cout << "Choose a cave to enter (1-" << caves.size() << "): ";
            std::cin >> caveChoice;

            if (caveChoice > 0 && caveChoice <= static_cast<int>(caves.size())) {
                Cave selectedCave = caves[caveChoice - 1];
                std::cout << "You entered the " << selectedCave.getName() << ".\n";
                for (auto& monster : selectedCave.getMonsters()) {
                    
                    if (hero.isAlive()) {
                        combat(hero, monster);
                        hero.resetHp(); // Reset hero's HP after the fight
                    
                    } else {
                        std::cout << "You have been defeated in the cave. Game over." << "\n";
                        break; 
                    }
                } 
                if (hero.isAlive()) {
                    hero.addGold(selectedCave.getGoldReward()); // Add gold reward after clearing the cave
                    std::cout << "You succesfully cleared the cave and earned " << selectedCave.getGoldReward() << " gold!" << "\n";
                }
            } else {
                std::cout << "Invalid choice." << "\n";
                break;
            }
        }

        else {
            std::cout << "Invalid choice. Please try again." << "\n";
        }
    }
}

void saveHero(const Hero& hero) {
    std::string filename = "hero_" + hero.getName() + ".txt";
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error opening file for saving." << std::endl;
        return;
    }

    file << hero.getName() << "\n"
         << hero.getHealth() << "\n"
         << hero.getStrength() << "\n"
         << hero.getXp() << "\n"
         << hero.getLevel() << "\n"
         << hero.getGold() << "\n"; // Save gold as well

    file.close();
}

Hero loadHero(const std::string& name) {
    std::string filename = "hero_" + name + ".txt";
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file for loading." << std::endl;
        return Hero("Default", 10, 2, 0, 1, 0); // fallback
    }

    std::string heroName;
    int health, strength, xp, level, gold;

    std::getline(file, heroName); // Reads the first line (name) and stores it in heroName.
    file >> health >> strength >> xp >> level >> gold; // Reads the rest of the lines and stores them in respective variables.

    file.close();
    return Hero(heroName, health, strength, xp, level, gold);
}


bool heroExists(const std::string& name) {
    std::ifstream file("hero_" + name + ".txt");
    return file.good();
}


Cave::Cave(std::string name, int goldReward, std::vector<Monster> monsters) {
    this->name = name;
    this->goldReward = goldReward;
    this->monsters = monsters;
}

std::string Cave::getName() const { return name; }
int Cave::getGoldReward() const { return goldReward; }
std::vector<Monster>& Cave::getMonsters() { return monsters; }

int Hero::getGold() const { return gold; }
void Hero::addGold(int amount) { gold += amount; }

std::vector<Cave> generateCaves(int heroLevel) {
    std::vector<Cave> caves;
    std::vector<Monster> monsters = getMonsters();

    // Generate caves based on hero level
    if (heroLevel < 3) {
        caves.push_back(Cave("Small Cave", 50, {monsters[0], monsters[1]}));
    } else if (heroLevel < 5) {
        caves.push_back(Cave("Goblin Cave", 100, {monsters[2], monsters[3]}));
    } else if (heroLevel < 7) {
        caves.push_back(Cave("Strong Goblin Cave", 150, {monsters[4], monsters[5]}));
    } else {
        caves.push_back(Cave("Dragon's Nest", 500, {monsters[6], monsters[7]}));
    }

    return caves;
}
