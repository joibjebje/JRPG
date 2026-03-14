#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "core/hero.h"
#include "core/enemy.h"
#include "world/world.h"

namespace jrpg {

std::vector<Hero> init_heroes();
std::unordered_map<std::string, Enemy> init_enemies();
std::vector<World> init_worlds(const std::unordered_map<std::string, Enemy>& enemies);


}