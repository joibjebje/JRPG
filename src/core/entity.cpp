#include "core/entity.h"
#include "ability/ability.h"

#include <string>
#include <stdexcept>

namespace jrpg {

    Entity::Entity(const std::string& _name, EntityType _type) {
        name = _name;
        type = _type;
        health = 100.0f;
        max_health = 100.0f;
        isDead = false;
    }

    void Entity::add_ability(const Ability& a) {
        abilities.push_back(a);
    }

    Ability& Entity::get_ability(const std::string& id) {
        for (auto& a : abilities) {
            if (a.id == id) return a;
        }
        throw std::out_of_range("Ability not found: " + id);
    }

    const std::vector<Ability>& Entity::get_abilities() const {
        return abilities;
    }

    std::vector<Ability>& Entity::get_abilities_mut() {
        return abilities;
    }

    void Entity::reset_abilities() {
        for (auto& a : abilities) {
            a.reset_for_battle();
        }
    }

    void Entity::tick_cooldowns() {
        for (auto& a : abilities) {
            a.tick_cooldown();
        }
    }

    void Entity::set_health(float new_health) {
        if (new_health > max_health) health = max_health;
        else health = new_health;
    }

    void Entity::add_health(float health_added) {
        health += health_added;
    }

    bool Entity::take_damage(int damage_taken) {
        health -= damage_taken;
        return health <= 0;
    }

    void Entity::die() {
        isDead = true;
    }

}
