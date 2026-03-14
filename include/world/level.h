#pragma once

#include <string>
#include <vector>
#include "battle/battle.h"

namespace jrpg {

class Level {
public:

    Level(int wi, int li);

    int world_idx, level_idx;
    std::string intro;

    void add_battle(Battle& battle);
    std::vector<Battle>& get_battles();
    int battle_count() const;

private:

    std::vector<Battle> battles;

};

}