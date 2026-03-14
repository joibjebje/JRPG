#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include "core/hero.h"
#include "core/enemy.h"
#include "world/world.h"

namespace jrpg {

class Game {
public:

    Game();

    void start(); // Main loop

    /**
     * Assigns a hero to the player's party.
     * Returns false if the party is full and the hero wasn't already in it.
     */
    bool assign_hero(int hero_idx);

    static const int MAX_PARTY_SIZE = 3;

    // Returns the number of heroes currently in the party
    int party_size() const;

    // Returns pointers to all heroes currently in the party
    std::vector<Hero*> get_party();

    // List of all heroes <hero_id, HeroObj>
    std::vector<Hero> heroes;

    // Enemy registry <enemy_id, EnemyObj>
    std::unordered_map<std::string, Enemy> enemies;

    // List of all worlds
    std::vector<World> worlds;

private:

    // Menu switching funcs
    void play();
    void world_menu();
    void hero_menu();
    void start_level(Level& level);
    void quit();

    // If this boolean is true then the program quits gracefully
    bool shouldQuit = false;

    // List of heroes in party (heroes that player wants to bring to battles)
    // ( indices of heroes in heroes vector )
    std::vector<int> party;
};

}
