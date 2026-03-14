#pragma once

#include <string>

#include "core/entity.h"

namespace jrpg {

class Hero : public Entity {
public:

    Hero(const std::string& name);

    bool inParty = false;

private:

};

}