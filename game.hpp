#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include <vector>

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
    
    public:
        Hero(std::string name, int health = 10, int strength = 2, int xp = 0,  int level = 1);
        
        int getXp() const;
        int getLevel() const;
        
        void gainXp(int amount);
        void levelUp();
        void resetHp(); // Reset HP to max after every fight

};

class Monster : public Character {
    
    private:
        int xpReward;

    public: 
        Monster(std::string name, int health, int strength, int xp_reward);
        
        int getXpReward() const;  
};


void combat(Hero &hero, Monster &monster);
std::vector<Monster> getMonsters();
std::vector<Hero> getHeroes();
void gameLoop(Hero& hero);
void saveHero(const Hero& hero);
Hero loadHero(const std::string& heroName);


#endif // GAME_HPP