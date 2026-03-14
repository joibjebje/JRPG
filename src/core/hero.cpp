#include <string>
#include "core/entity.h"
#include "core/hero.h"

namespace jrpg {

    Hero::Hero(const std::string& _name) : Entity(_name, EntityType::HERO) { }


}