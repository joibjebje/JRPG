#include "world/level.h"

namespace jrpg {

Level::Level(int wi, int li) {
    world_idx = wi;
    level_idx = li;
}

void Level::add_battle(Battle& battle) {
    battles.push_back(battle);
}

std::vector<Battle>& Level::get_battles() {
    return battles;
}

int Level::battle_count() const {
    return static_cast<int>(battles.size());
}

}
