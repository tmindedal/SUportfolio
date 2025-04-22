#ifndef GAME_HPP
#define GAME_HPP

#include <string>


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
        Hero(std::string name, int health, int strength, int level);
        
        int getXp() const;
        int getLevel() const;
        
        void gainXp(int amount);
        void levelUp();

};



#endif // GAME_HPP