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
std::string Character::getName() const {
    return name;
}

int Character::getHealth() const {
    return hp;
}

int Character::getStrength() const {
    return strength;
}

void Character::damage(int amount) {
    hp -= amount;
}

bool Character::isAlive() const {
    return hp > 0;
}


