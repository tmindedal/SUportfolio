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
}

// Defining the functions of the Hero class.
int Hero::getXp() const { return xp; }

int Hero::getLevel() const { return level; }

void Hero::gainXp(int amount) { xp += amount; }

void Hero::levelUp() {
    if (xp >= level * 1000) {
        level++;
        strength += 1;
        hp += 2;
        xp -= level * 1000; // Reset XP after leveling up
        std::cout << name << " leveled up to level " << level << "\n";
        std::cout << "New strength: " << strength << "\n";
        std::cout << "New health: " << hp << "\n";
    }
}

Monster::Monster(std::string name, int health, int strength, int xpReward) : Character(name, health, strength) {
    this->name = name;
    this->hp = health;
    this->strength = strength;
    this->xpReward = xpReward;
}

int Monster::getXpReward() const { return xpReward; }
