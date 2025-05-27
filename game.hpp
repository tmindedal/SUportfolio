#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include <vector>
extern "C" {
    #include "sqlite3.h"
}

class Character {
    
    protected:
        std::string name;    
        int hp;
        int strength;
    
    public:
        Character(std::string name, int health, int strength);
        virtual ~Character() = default;

        std::string getName() const;
        int getHealth() const;
        int getStrength() const;
       
        void damage(int amount);
        bool isAlive() const;
};


class Hero : public Character {
    
    private:
        int level;
        int xp;
        int maxHp; // To reset HP after every fight
        int gold; // Added gold attribute for the hero for second iteration.

        Weapon weapon; // Weapon attribute for the hero
    
    public:
        Hero(std::string name, int health = 10, int strength = 2, int xp = 0,  int level = 1, int gold = 0, int maxHp = 10);
        
        int getXp() const;
        int getLevel() const;
        int getMaxHp() const; // Getter for max HP
        
        void gainXp(int amount);
        void levelUp();
        void resetHp(); // Reset HP to max after every fight
        int getGold() const; // Getter for gold
        void addGold(int amount);// Method to add gold

        Weapon getWeapon() const { return weapon; } // Getter for weapon
        void setWeapon(const Weapon& newWeapon) { weapon = newWeapon; } // Setter for weapon
};

class Monster : public Character {
    
    private:
        int xpReward;

    public: 
        Monster(std::string name, int health, int strength, int xp_reward);
        
        int getXpReward() const;  
};


class Cave {
    private:
        std::string name;
        int goldReward;
        std::vector<Monster> monsters;

    public:
        Cave(std::string name, int goldReward, std::vector<Monster> monsters);
        
        std::string getName() const;
        int getGoldReward() const;
        std::vector<Monster>& getMonsters();

};

std::vector<Cave> generateCaves(int heroLevel);


void combat(Hero &hero, Monster &monster);
std::vector<Monster> getMonsters();
std::vector<Hero> getHeroes();
void gameLoop(Hero& hero);
// void saveHero(const Hero& hero);
// Hero loadHero(const std::string& heroName);
// bool heroExists(const std::string& name);



// SQLite database functions
void initDatabase();
void saveHeroToDatabase(const Hero& hero);
Hero loadHeroFromDatabase(const std::string& name);
bool heroExistsInDatabase(const std::string& name);



// Weapon Class
class Weapon {
    private:
        std::string name;
        int damage;
        int strengthModifier;
        int durability;

    public:
        Weapon(std::string name ="None", int dmg = 0, int strMod = 0, int dur = 0) {
            this ->name = name;
            this ->damage = dmg;
            this ->strengthModifier = strMod;
            this ->durability = dur;
        }

        std::string getName() const { return name; }
        int getDamage() const { return damage; }
        int getStrengthModifier() const { return strengthModifier; }
        int getDurability() const { return durability; }

        void reduceDurability(int amount) {
            if (durability > 0) durability--; // Ensure durability doesn't go below 0
        }

        // Check if the weapon is broken
        bool isBroken() const { return durability <= 0; }

        void setName(std::string newName) { name = newName; }
        void setDamage(int newDamage) { damage = newDamage; }
        void setStrengthModifier(int newStrengthModifier) { strengthModifier = newStrengthModifier; }
        void setDurability(int newDurability) { durability = newDurability; }
};




#endif // GAME_HPP