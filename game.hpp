#ifndef GAME_HPP
#define GAME_HPP

#include <string>


class character {
    
    protected:
        std::string name;    
        int hp;
        int strength;
    
    public:
        Character()(std::string name, int health, int strength);
        virtual ~Character() = default;

        std::string getName() const;
        int getHealth() const;
        int getStrength() const;
        void damage(int amount);
        bool isAlive() const;
};





#endif // GAME_HPP