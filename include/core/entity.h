#pragma once

#include <string>
#include <vector>

#include "ability/ability.h"

namespace jrpg {

enum EntityType {
    HERO, ENEMY, BOSS
};

class Entity {
public:

    std::string name;
    EntityType type;

    float health;
    float max_health;

    bool isDead;

    Entity(const std::string& name, EntityType type);

    void add_ability(const Ability& a);
    Ability& get_ability(const std::string& id);
    const std::vector<Ability>& get_abilities() const;
    std::vector<Ability>& get_abilities_mut();

    void reset_abilities();
    void tick_cooldowns();

    void set_health(float new_health);
    void add_health(float health_added);

    bool take_damage(int damage_taken);
    void die();

private:

    std::vector<Ability> abilities;

};

}
