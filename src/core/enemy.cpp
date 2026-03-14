#include <string>
#include "core/entity.h"
#include "core/enemy.h"

namespace jrpg {

    Enemy::Enemy(const std::string& _name) : Entity(_name, EntityType::ENEMY) { }


}