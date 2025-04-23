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
        int xpToLevelUp;
    
    public:
        Hero(std::string name, int health = 10, int strength = 2, int xp = 0,  int level = 1);
        
        int getXp() const;
        int getLevel() const;
        
        void gainXp(int amount);
        void levelUp();

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
void gameLoop(Hero& hero);


#endif // GAME_HPP