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
Hero::Hero(std::string name, int health, int strength, int xp, int level) : Character(name, health, strength) {
    this->name = name;
    hp = health;
    this->strength = strength;
    this->level = level;
    this->xp = xp;
    maxHp = health; // Set max HP so we can reset the HP after every fight
}

// Defining the functions of the Hero class.
int Hero::getXp() const { return xp; }

int Hero::getLevel() const { return level; }

void Hero::gainXp(int amount) { xp += amount; }

void Hero::levelUp() {
    if (xp >= level * 1000) {
        level++;
        strength++;
        maxHp += 2;
        xp -= level * 1000; // Reset XP after leveling up
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
        std::cout << "Press 0 to exit or any other key to continue: ";
        char choice;
        std::cin >> choice;

        if (choice == '0') break;
        else {
            std::cout << "You continue your adventure!" << "\n\n";
            std::cout << "List of opponents: " << "\n";
            std::vector<Monster> monsters = getMonsters();
            for (int i = 0; i < monsters.size(); i++) {
                std::cout << i + 1 << ". " << monsters[i].getName() << ", " << monsters[i].getHealth() << " health, " 
                << monsters[i].getStrength() << " strength, " << "\n";
            }
            std::cout << "\nChoose your opponent: ";
            int monsterChoice;
            std::cin >> monsterChoice;
            if (monsterChoice > 0 && monsterChoice <= monsters.size()) {
                std::cout << "\nYou chose " << monsters[monsterChoice - 1].getName() << "!" << "\n";
                combat(hero, monsters[monsterChoice - 1]);
                hero.resetHp(); // Reset HP after every fight
            }
        }
    }
}

void saveHero(const Hero& hero) {
    std::ofstream file("hero_save.txt");
    if (!file) {
        std::cerr << "Error opening file for saving." << std::endl;
        return;
    }
    else {
        file << hero.getName() << "\n";
        file << hero.getHealth() << "\n";
        file << hero.getStrength() << "\n";
        file << hero.getXp() << "\n";
        file << hero.getLevel() << "\n";
        file.close();
    } 
}

Hero loadHero() {
    std::ifstream file("hero_save.txt");
    if (!file) {
        std::cerr << "Error opening file for loading." << std::endl;
        return;
    }
    else {
        std::string name;
        int health, strength, xp, level;
        file >> name >> health >> strength >> xp >> level;
        Hero hero = Hero(name, health, strength, xp, level);
        file.close();
    }
}