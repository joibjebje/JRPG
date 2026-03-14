#pragma once

#include "world/level.h"

#include <vector>
#include <string>

namespace jrpg {

class World {
public:

    World(const std::string& name);

    std::string name;
    bool completed = false;

    void add_level(Level& level);
    Level& get_level(int level);
    int level_count() const;

private:

    std::vector<Level> levels;
};

}