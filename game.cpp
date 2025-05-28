#include "game.hpp"
#include <iostream>
#include <fstream>
extern "C" {
    #include "sqlite3.h"
}


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
Hero::Hero(std::string name, int health, int strength, int xp, int level, int gold, int maxHp) : Character(name, health, strength) {
    hp = health;
    this->strength = strength;
    this->level = level;
    this->xp = xp;
    this->maxHp = maxHp; // Set max HP so we can reset the HP after every fight
    this->gold = gold; // Initialize gold
}

// Defining the functions of the Hero class.
int Hero::getXp() const { return xp; }

int Hero::getLevel() const { return level; }

int Hero::getMaxHp() const { return maxHp; } // Getter for max HP

void Hero::gainXp(int amount) { xp += amount; }

void Hero::levelUp() {
    if (xp >= level * 1000) {
        strength++;
        maxHp += 2;
        xp -= level * 1000; // Reset XP after leveling up
        level++;
        std::cout << this->name << " leveled up to level " << level << "\n";
        std::cout << "New strength: " << strength << "\n";
        std::cout << "New health: " << maxHp << "\n\n";
    }
}

void Hero::resetHp() { 
    this->hp = maxHp; // Reset HP to max after every fight
} 

Monster::Monster(std::string name, int health, int strength, int xpReward) : Character(name, health, strength) {
    this->hp = health;
    this->strength = strength;
    this->xpReward = xpReward;
}

int Monster::getXpReward() const { return xpReward; }


void combat(Hero &hero, Monster &monster) {
    std::cout << "The battle between " << hero.getName() << " and " << monster.getName() << " begins! " << "\n\n";

    while (hero.isAlive() && monster.isAlive()) {
        int totalStrength = hero.getStrength() + hero.getWeapon().getDamage() + ( hero.getLevel() * hero.getWeapon().getStrengthModifier() );
        std::cout << hero.getName() << " attacks " << monster.getName() << " and deals " << totalStrength << " damage!" << "\n";
        monster.damage(totalStrength);
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
        
        // getWeapon() is a const method so im making a copy and setting it as the new weapon to reduce durability.
        Weapon currentWeapon = hero.getWeapon();
        currentWeapon.setDurability(currentWeapon.getDurability() - 1); // Reduce weapon durability by 1
        hero.setWeapon(currentWeapon); // Set the new weapon with reduced durability 
        std::cout << "Your weapon's durability is now: " << hero.getWeapon().getDurability() << "\n";

        // Register the kill in the database
        registerKillInDatabase(hero.getName(), hero.getWeapon().getName());


    } else {
        std::cout << "You lost the battle " << hero.getName() << "!" << "\n\n";
        
        // same thing
        Weapon currentWeapon = hero.getWeapon();
        currentWeapon.setDurability(currentWeapon.getDurability() - 1); // Reduce weapon durability by 1
        hero.setWeapon(currentWeapon); // Set the new weapon with reduced durability 
        std::cout << "Your weapon's durability is now: " << hero.getWeapon().getDurability() << "\n";
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
        std::cout << "Press 0 to exit, 1 to fight a monster or 2 to enter a cave: " << "\n";
        char choice;
        std::cin >> choice;

        if (choice == '0') break;
        
        else if (choice == '1') {
            std::vector<Monster> monsters = getMonsters();
            for (size_t i = 0; i < monsters.size(); ++i) {
                std::cout << i + 1 << ". " << monsters[i].getName() << " (Health: " << monsters[i].getHealth() << ", Strength: " << monsters[i].getStrength() << ")\n";
            }
            int monsterChoice;
            std::cout << "Choose a monster to fight (1-" << monsters.size() << "): ";
            std::cin >> monsterChoice;
            if (monsterChoice > 0 && monsterChoice <= static_cast<int>(monsters.size())) {
                combat(hero, monsters[monsterChoice - 1]);
                hero.resetHp(); // Reset hero's HP after the fight
            } else {
                std::cout << "Invalid choice." << "\n";
                continue; // Skip to the next iteration of the loop;
            }
        }

        else if (choice == '2') {
            std::vector<Cave> caves = generateCaves(hero.getLevel());
            for (size_t i = 0; i < caves.size(); ++i) {
                std::cout << i + 1 << ". " << caves[i].getName() << " (Gold Reward: " << caves[i].getGoldReward() << ")\n";
            }
            int caveChoice;
            std::cout << "Choose a cave to enter (1-" << caves.size() << "): ";
            std::cin >> caveChoice;

            if (caveChoice > 0 && caveChoice <= static_cast<int>(caves.size())) {
                Cave selectedCave = caves[caveChoice - 1];
                bool caveCleared = true;
                std::cout << "You entered the " << selectedCave.getName() << ".\n";
                for (auto& monster : selectedCave.getMonsters()) {
                    combat(hero, monster);
                    
                    
                    if (!hero.isAlive()) {
                        std::cout << "You have been defeated in the cave. Game over." << "\n";
                        caveCleared = false;
                        saveHeroToDatabase(hero); // Save hero before exiting
                        break; 
                    }
                    hero.resetHp();
                    
                } 

                if (caveCleared && hero.isAlive()) {
                    hero.addGold(selectedCave.getGoldReward()); // Add gold reward after clearing the cave
                    std::cout << "You succesfully cleared the cave and earned " << selectedCave.getGoldReward() << " gold!" << "\n";

                    if (selectedCave.getName() == "Weak Horse Cave") { hero.setWeapon(Weapon("Wooden Sword", 2, 0, 10)); }
                    else if (selectedCave.getName() == "Weak Goblin Cave") { hero.setWeapon(Weapon("Rusty Sword", 3, 0, 10)); }
                    else if (selectedCave.getName() == "Goblin Camp") { hero.setWeapon(Weapon("Steel Sword", 4, 0, 10)); }
                    else if (selectedCave.getName() == "Goblin Cave") { hero.setWeapon(Weapon("Silver Sword", 5, 0, 10)); }
                    else if (selectedCave.getName() == "Strong Goblin Cave") { hero.setWeapon(Weapon("Gold Sword", 6, 0, 10)); }
                    else if (selectedCave.getName() == "Mystical Cave") { hero.setWeapon(Weapon("Mystic Sword", 7, 0, 10)); }
                    else if (selectedCave.getName() == "Monkey Cave") { hero.setWeapon(Weapon("Legendary Sword", 8, 0, 10)); }
                    else if (selectedCave.getName() == "Dragon's Nest") { hero.setWeapon(Weapon("Dragon Slayer", 10, 0, 10)); }
                    
                    std::cout << "You found a new weapon: " << hero.getWeapon().getName() << " with " << hero.getWeapon().getDamage() << " damage!" << "\n";
                }

            } else {
                std::cout << "Invalid choice." << "\n";
                ;
            }
        }

        else {
            std::cout << "Invalid choice. Please try again." << "\n";
        }
    }
}

/*void saveHero(const Hero& hero) {
    std::string filename = "hero_" + hero.getName() + ".txt";
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error opening file for saving." << std::endl;
        return;
    }

    file << hero.getName() << "\n"
         << hero.getHealth() << "\n"
         << hero.getStrength() << "\n"
         << hero.getXp() << "\n"
         << hero.getLevel() << "\n"
         << hero.getGold() << "\n" // Save gold as well
         << hero.getMaxHp() << "\n"; // Save max HP

    file.close();
}

Hero loadHero(const std::string& name) {
    std::string filename = "hero_" + name + ".txt";
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file for loading." << std::endl;
        return Hero("Default", 10, 2, 0, 1, 0); // fallback
    }

    std::string heroName;
    int health, strength, xp, level, gold, maxHp;

    std::getline(file, heroName); // Reads the first line (name) and stores it in heroName.
    file >> health >> strength >> xp >> level >> gold >> maxHp; // Reads the rest of the lines and stores them in respective variables.

    file.close();
    return Hero(heroName, health, strength, xp, level, gold, maxHp); // Returns a Hero object with the loaded data.
}


bool heroExists(const std::string& name) {
    std::ifstream file("hero_" + name + ".txt");
    return file.good();
}
*/

Cave::Cave(std::string name, int goldReward, std::vector<Monster> monsters) {
    this->name = name;
    this->goldReward = goldReward;
    this->monsters = monsters;
}

std::string Cave::getName() const { return name; }
int Cave::getGoldReward() const { return goldReward; }
std::vector<Monster>& Cave::getMonsters() { return monsters; }

int Hero::getGold() const { return gold; }
void Hero::addGold(int amount) { gold += amount; }

std::vector<Cave> generateCaves(int heroLevel) {
    std::vector<Cave> caves;
    std::vector<Monster> m = getMonsters();

    // Generate caves based on hero level
    if (heroLevel <= 3) {
        caves.push_back(Cave("Weak Horse Cave", 50, {m[0], m[0], m[0]}));
        caves.push_back(Cave("Weak Goblin Cave", 50, {m[1], m[1], m[1]}));
        caves.push_back(Cave("Goblin Camp", 50, {m[1], m[2], m[2]}));
    } else if (heroLevel <= 5) {
        caves.push_back(Cave("Goblin Cave", 100, {m[2], m[2], m[2]}));
        caves.push_back(Cave("Strong Goblin Cave", 100, {m[3], m[3], m[3]}));
        caves.push_back(Cave("Mystical Cave", 100, {m[1], m[3], m[2]}));
    } else if (heroLevel <= 7) {
        caves.push_back(Cave("Mystical Cave", 100, {m[1], m[3], m[2]}));
        caves.push_back(Cave("Strong Goblin Cave", 150, {m[4], m[5], m[4]}));
        caves.push_back(Cave("Monkey Cave", 150, {m[5], m[5], m[5]}));
    } else {
        caves.push_back(Cave("Strong Goblin Cave", 150, {m[4], m[5], m[4]}));
        caves.push_back(Cave("Monkey Cave", 150, {m[5], m[5], m[5]}));
        caves.push_back(Cave("Dragon's Nest", 500, {m[7]}));
    }

    return caves;
}


// Implementation of the database functions

void initDatabase() {
    sqlite3* db;

    // Åben eller opret database med navnet 'heroes.db'
    int res = sqlite3_open("heroes.db", &db);
    if (res != SQLITE_OK) {
        std::cerr << "Error while opening database. \n";
        return;
    }

    // Opret tabel hvis den ikke allerede findes
    const char* createTableSQL = 
        "CREATE TABLE IF NOT EXISTS heroes ("
        "name TEXT PRIMARY KEY, "
        "health INTEGER, "
        "strength INTEGER, "
        "xp INTEGER, "
        "level INTEGER, "
        "gold INTEGER, "
        "maxHp INTEGER, "
        "weapon_name TEXT DEFAULT 'None', "
        "weapon_damage INTEGER DEFAULT 0, "
        "weapon_modifier INTEGER DEFAULT 0, "
        "weapon_durability INTEGER DEFAULT 0"
        ");";
    
    sqlite3_exec(db, createTableSQL, nullptr, nullptr, nullptr);

     const char* createKillsSQL =
        "CREATE TABLE IF NOT EXISTS kills ("
        "heroName TEXT, "
        "weaponName TEXT, "
        "kills INTEGER DEFAULT 0, "
        "PRIMARY KEY (heroName, weaponName));";

    sqlite3_exec(db, createKillsSQL, nullptr, nullptr, nullptr);

    sqlite3_close(db); // Luk databaseforbindelsen
}

void saveHeroToDatabase(const Hero& hero) {
    sqlite3* db; // Pointer to the SQLite database
    sqlite3_open("heroes.db", &db); // Åbner databasen

    // Indsæt eller opdater helten i databasen
    // Hvis helten allerede findes, opdateres den; ellers indsættes den.
    const char* sql = "INSERT OR REPLACE INTO heroes "
                      "(name, health, maxHp, strength, xp, level, gold, "
                      "weapon_name, weapon_damage, weapon_modifier, weapon_durability) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
    
    sqlite3_stmt* stmt; 
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    // Gem heltens attributter i databasen.
    sqlite3_bind_text(stmt, 1, hero.getName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, hero.getHealth());
    sqlite3_bind_int(stmt, 3, hero.getMaxHp());
    sqlite3_bind_int(stmt, 4, hero.getStrength());
    sqlite3_bind_int(stmt, 5, hero.getXp());
    sqlite3_bind_int(stmt, 6, hero.getLevel());
    sqlite3_bind_int(stmt, 7, hero.getGold());

    // Gem våben attributter i databasen.
    sqlite3_bind_text(stmt, 8, hero.getWeapon().getName().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 9, hero.getWeapon().getDamage());
    sqlite3_bind_int(stmt, 10, hero.getWeapon().getStrengthModifier());
    sqlite3_bind_int(stmt, 11, hero.getWeapon().getDurability());


    sqlite3_step(stmt); // Gem eller opdater helten i databasen
    sqlite3_finalize(stmt); 
    sqlite3_close(db); // Luk databaseforbindelsen
}


Hero loadHeroFromDatabase(const std::string& name) {

    Hero hero(name, 10, 2, 0, 1, 0, 10); // Backup helt

    sqlite3* db;
    sqlite3_open("heroes.db", &db); // Åben database

   const char* sqp =
        "SELECT name, health, strength, xp, level, gold, maxHp, "
        "weapon_name, weapon_damage, weapon_modifier, weapon_durability "
        "FROM heroes WHERE name = ?;"; 

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sqp, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        // Hent data fra databasen
        std::string heroName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        int health = sqlite3_column_int(stmt, 1);
        int strength = sqlite3_column_int(stmt, 2);
        int xp = sqlite3_column_int(stmt, 3);
        int level = sqlite3_column_int(stmt, 4);
        int gold = sqlite3_column_int(stmt, 5);
        int maxHp = sqlite3_column_int(stmt, 6);

        // Hent våben data fra databasen
        std::string weaponName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));
        int weaponDamage = sqlite3_column_int(stmt, 8);
        int weaponModifier = sqlite3_column_int(stmt, 9);
        int weaponDurability = sqlite3_column_int(stmt, 10);


        // Opret en Hero-objekt og weapon-objekt med de hentede data
        Weapon weapon(weaponName, weaponDamage, weaponModifier, weaponDurability);
        hero = Hero(heroName, health, strength, xp, level, gold, maxHp);
        hero.setWeapon(weapon); // Sæt våben for helten
    }

    sqlite3_finalize(stmt); 
    sqlite3_close(db); // Luk databaseforbindelsen
    return hero; // Returner helten

}


bool heroExistsInDatabase(const std::string& name) {
    // Åben databaseforbindelsen.
    sqlite3* db;
    sqlite3_open("heroes.db", &db); 

    const char* sql = "SELECT COUNT(*) FROM heroes WHERE name = ?;";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);

    bool exists = false;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        exists = sqlite3_column_int(stmt, 0) > 0; // Hvis antallet er større end 0, findes helten
    }

    sqlite3_finalize(stmt); 
    sqlite3_close(db); // Luk databaseforbindelsen
    return exists; // Returner om helten findes

}

void registerKillInDatabase(const std::string& heroName, const std::string& weaponName) {
    sqlite3* db;
    sqlite3_open("heroes.db", &db);

    const char* sql = 
        "INSERT INTO kills (heroName, weaponName, kills) "
        "VALUES (?, ?, 1) "
        "ON CONFLICT(heroName, weaponName) DO UPDATE SET kills = kills + 1;";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, heroName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, weaponName.c_str(), -1, SQLITE_STATIC);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}


// Funktioner til at analysere data fra databasen

void showHereoesAlphabetically() {

    // Åben databaseforbindelsen.
    sqlite3* db;
    sqlite3_open("heroes.db", &db);

    // SQL-forespørgsel for at hente helte sorteret alfabetisk
    const char* sql = "SELECT name FROM heroes ORDER BY name ASC;";

    // Forbered SQL-forespørgslen
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    // Udfør forespørgslen og udskriv resultaterne
    std::cout << "Heroes sorted alphabetically:\n";
    while (sqlite3_step(stmt) == SQLITE_ROW) {

        // Hent navne og print dem
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::cout << "- " << name << "\n";

    }
    sqlite3_finalize(stmt); // Fjern prepared statement
    sqlite3_close(db); // Luk databaseforbindelsen
}

void showTotalKills() {
    // Åben databaseforbindelsen.
    sqlite3* db;
    sqlite3_open("heroes.db", &db);

    // SQL-forespørgsel som summer alle kills for hver helt.
    const char* sql = "SELECT heroName, SUM(kills) FROM kills GROUP BY heroName;";

    // Forbered SQL-forespørgslen
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    std::cout << "Total kills by each hero:\n";
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // Hent heltenavn (0) og antal kills (1) og print dem.
        std::string heroName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        int totalKills = sqlite3_column_int(stmt, 1);
        std::cout << heroName << ": " << totalKills << " kills\n";
    }

    sqlite3_finalize(stmt); // Fjern prepared statement
    sqlite3_close(db); // Luk databaseforbindelsen

}

void showTopHeroForWeapon() {

    // Åben databaseforbindelsen.
    sqlite3* db;
    sqlite3_open("heroes.db", &db);

    // SQL-forespørgsel for at hente helten med flest kills for hver våben.
    const char* sql = 
        "SELECT heroName, weaponName, MAX(kills) "
        "FROM kills "
        "GROUP BY weaponName;";

    // Forbered SQL-forespørgslen
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    std::cout << "Top hero for each weapon:\n";

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // Hent heltenavn (0), våben navn (1) og antal kills (2) og print dem.
        std::string heroName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string weaponName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        int totalKills = sqlite3_column_int(stmt, 2);

        std::cout << heroName << " with " << weaponName << ": " << totalKills << " kills\n";
    }

    sqlite3_finalize(stmt); // Fjern prepared statement
    sqlite3_close(db); // Luk databaseforbindelsen

}

void showKillsPerWeapon(const std::string& heroName) {
    // Åben databaseforbindelsen.
    sqlite3* db;
    sqlite3_open("heroes.db", &db);

    // SQL-forespørgsel for at hente antal kills for hver våben for en specifik helt.
    const char* sql = 
        "SELECT weaponName, kills "
        "FROM kills "
        "WHERE heroName = ?;";

    // Forbered SQL-forespørgslen
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    // Bind heltenavn til forespørgslen
    sqlite3_bind_text(stmt, 1, heroName.c_str(), -1, SQLITE_STATIC);

    std::cout << "Kills per weapon for " << heroName << ":\n";

    // Udfør forespørgslen og udskriv resultaterne
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string weapon = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        int kills = sqlite3_column_int(stmt, 1);

        // Print våben og antal kills
        std::cout << weapon << ": " << kills << " kills\n";

    }

    sqlite3_finalize(stmt); // Fjern prepared statement
    sqlite3_close(db); // Luk databaseforbindelsen
}
