#include "game.hpp"
#include <iostream>
#include <fstream>


// Implementation of the Character class
// Constructor
Character::Character(std::string name, int health, int strength) {
    this->name = name;
    this->hp = health;
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
    this->hp = health;
    this->strength = strength;
    this->level = level;
    this->xp = xp;
    this->maxHp = health; // Set max HP so we can reset the HP after every fight
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
        std::cout << "New health: " << hp << "\n";
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
    std::cout << "The battle between " << hero.getName() << " and " << monster.getName() << " begins! " << "\n";
    while (hero.isAlive() && monster.isAlive()) {
        std::cout << hero.getName() << " attacks " << monster.getName() << " and deals " << hero.getStrength() << " damage!" << "\n";
        monster.damage(hero.getStrength());
        std::cout << monster.getName() << " has " << monster.getHealth() << " health left." << "\n";
        if (!monster.isAlive()) break;

        std::cout << monster.getName() << " attacks " << hero.getName() << " and deals " << monster.getStrength() << " damage!" << "\n";
        hero.damage(monster.getStrength());
        std::cout << hero.getName() << " has " << hero.getHealth() << " health left." << "\n";
    }

    if (hero.isAlive()) {
        std::cout << "You win the battle " << hero.getName() << "!" << "\n";
        hero.gainXp(monster.getXpReward());
        std::cout << hero.getName() << " gained " << monster.getXpReward() << " XP!" << "\n";
    } else {
        std::cout << "You lost the battle " << hero.getName() << "!" << "\n";
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

void gameLoop(Hero& hero) {
    while (true) {
        std::cout << "Press 1 to exit or any other key to continue: ";
        int choice;
        std::cin >> choice;

        if (choice == 1) break;
        else {
            std::cout << "You continue your adventure!" << "\n";
            std::vector<Monster> monsters = getMonsters();
            for (int i = 0; i < monsters.size(); i++) {
                std::cout << i + 1 << ". " << monsters[i].getName() << ", " << monsters[i].getHealth() << " health, " 
                << monsters[i].getStrength() << " strength, " << "\n";
            }
            std::cout << "Choose your opponent: " << "\n";
            int monsterChoice;
            std::cin >> monsterChoice;
            if (monsterChoice > 0 && monsterChoice <= monsters.size()) {
                std::cout << "You chose " << monsters[monsterChoice - 1].getName() << "!" << "\n";
                combat(hero, monsters[monsterChoice - 1]);
                hero.resetHp(); // Reset HP after every fight
            }
        }
    }
}