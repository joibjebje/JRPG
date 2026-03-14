#pragma once

#include <string>
#include <vector>

namespace jrpg {

class Entity; // forward declaration

enum AbilityType {
    DAMAGE,
    HEAL
};

enum TargetType {
    SINGLE,  // one target
    AOE,     // all enemies or all allies
    SELF     // caster only
};

class Ability {
public:

    Ability(const std::string& id, const std::string& name,
            AbilityType type, TargetType target, int power,
            int max_uses = -1, int cooldown = 0, bool charged_at_start = true);

    std::string id;
    std::string name;
    AbilityType type;
    TargetType target;
    int power;

    // Usage limits: -1 = unlimited
    int max_uses;
    int uses_left;

    // Cooldown: 0 = no cooldown, N = wait N turns after use
    int cooldown;
    int cooldown_left;

    // Whether the ability starts ready at the beginning of a battle
    bool charged_at_start;

    // Returns true if the ability can be used right now
    bool is_available() const;

    // Call after using the ability
    void use();

    // Call at the start of each turn to tick down cooldowns
    void tick_cooldown();

    // Reset usage and cooldown for a new battle
    void reset_for_battle();

    void execute(Entity& caster, std::vector<Entity*>& targets);

    std::string type_str() const;
    std::string target_str() const;
    std::string status_str() const;

private:

};

}
