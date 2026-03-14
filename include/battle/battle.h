#pragma once

#include <vector>

#include "core/enemy.h"
#include "core/hero.h"

namespace jrpg {

class Battle {
public:

    std::vector<Enemy> enemies;
    std::string intro;

    // Runs the battle with the given party heroes
    // Returns true if the party won, false if they lost
    bool run(std::vector<Hero*>& party);

private:
};

}
