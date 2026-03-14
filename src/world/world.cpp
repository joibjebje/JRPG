#include "world/world.h"

namespace jrpg {

    World::World(const std::string& _name) {
        name = _name;
    }

    void World::add_level(Level& level) {
        levels.push_back(level);
    }

    Level& World::get_level(int level) {
        return levels.at(level);
    }

    int World::level_count() const {
        return static_cast<int>(levels.size());
    }

}
