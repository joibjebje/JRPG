#include "ability/ability.h"
#include "core/entity.h"

#include <iostream>

namespace jrpg {

Ability::Ability(const std::string& _id, const std::string& _name,
                 AbilityType _type, TargetType _target, int _power,
                 int _max_uses, int _cooldown, bool _charged_at_start)
    : id(_id), name(_name), type(_type), target(_target), power(_power),
      max_uses(_max_uses), uses_left(_max_uses),
      cooldown(_cooldown), cooldown_left(0),
      charged_at_start(_charged_at_start) {}

bool Ability::is_available() const {
    // Out of uses
    if (max_uses != -1 && uses_left <= 0) return false;
    // Still on cooldown
    if (cooldown_left > 0) return false;
    return true;
}

void Ability::use() {
    if (max_uses != -1) uses_left--;
    if (cooldown > 0) cooldown_left = cooldown;
}

void Ability::tick_cooldown() {
    if (cooldown_left > 0) cooldown_left--;
}

void Ability::reset_for_battle() {
    uses_left = max_uses;
    if (charged_at_start) {
        cooldown_left = 0;
    } else {
        cooldown_left = cooldown;
    }
}

void Ability::execute(Entity& caster, std::vector<Entity*>& targets) {

    switch (type) {

        case DAMAGE: {
            if (target == SINGLE) {
                // Hit the first target
                if (!targets.empty()) {
                    bool died = targets[0]->take_damage(power);
                    std::cout << caster.name << " uses " << name
                              << " on " << targets[0]->name
                              << " for " << power << " damage!\n";
                    if (died) {
                        targets[0]->die();
                        std::cout << targets[0]->name << " has been defeated!\n";
                    }
                }
            } else if (target == AOE) {
                // Hit all targets
                std::cout << caster.name << " uses " << name
                          << " on all enemies for " << power << " damage!\n";
                for (Entity* t : targets) {
                    bool died = t->take_damage(power);
                    if (died) {
                        t->die();
                        std::cout << t->name << " has been defeated!\n";
                    }
                }
            } else if (target == SELF) {
                std::cout << caster.name << " uses " << name << " on himself for " << power << " damage!\n";
                bool died = caster.take_damage(static_cast<float>(power));
                if (died) {
                    caster.die();
                    std::cout << caster.name << " has been defeated!\n";
                }
            }
            break;
        }

        case HEAL: {
            if (target == SELF) {
                caster.add_health(static_cast<float>(power));
                if (caster.health > caster.max_health)
                    caster.health = caster.max_health;
                std::cout << caster.name << " uses " << name
                          << " and heals for " << power << " HP!\n";
            } else if (target == SINGLE) {
                // Heal first ally in targets
                if (!targets.empty()) {
                    targets[0]->add_health(static_cast<float>(power));
                    if (targets[0]->health > targets[0]->max_health)
                        targets[0]->health = targets[0]->max_health;
                    std::cout << caster.name << " uses " << name
                              << " on " << targets[0]->name
                              << " and heals for " << power << " HP!\n";
                }
            } else if (target == AOE) {
                // Heal all allies
                std::cout << caster.name << " uses " << name
                          << " and heals all allies for " << power << " HP!\n";
                for (Entity* t : targets) {
                    t->add_health(static_cast<float>(power));
                    if (t->health > t->max_health)
                        t->health = t->max_health;
                }
            }
            break;
        }

    }

    use();

}

std::string Ability::type_str() const {
    switch (type) {
        case DAMAGE: return "Damage";
        case HEAL:   return "Heal";
    }
    return "Unknown";
}

std::string Ability::target_str() const {
    switch (target) {
        case SINGLE: return "Single";
        case AOE:    return "AOE";
        case SELF:   return "Self";
    }
    return "Unknown";
}

std::string Ability::status_str() const {
    if (max_uses != -1 && uses_left <= 0) return "NO USES";
    if (cooldown_left > 0) return "CD:" + std::to_string(cooldown_left);
    return "READY";
}

}
