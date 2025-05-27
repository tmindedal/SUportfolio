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



#endif // GAME_HPP