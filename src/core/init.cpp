#include <vector>
#include <unordered_map>
#include <string>

#include "core/entity.h"
#include "core/hero.h"
#include "ability/ability.h"
#include "world/world.h"
#include "world/level.h"
#include "battle/battle.h"
#include "core/enemy.h"

namespace jrpg {

std::vector<Hero> init_heroes() {

    std::vector<Hero> heroes;

    // New Hero Structure:
    // Hero hero = Hero(hero_name);
    // hero.health = hero.max_health = xxx.0f;
    // hero.add_ability(Ability(a_id, a_name, a_type, a_target, a_amount, a_max_uses, a_cooldown, a_charged));
    // heroes.emplace_back(hero);

    {
        Hero hero = Hero("Pubert");
        hero.health = hero.max_health = 20.0f;
        hero.add_ability(Ability("throw_poop",       "Throw Poop",       DAMAGE, SINGLE, 10));
        hero.add_ability(Ability("tailwind",         "Tailwind",    DAMAGE, AOE,    5));
        heroes.emplace_back(hero);
    }
    {
        Hero hero = Hero("Patrick Bateman");
        hero.health = hero.max_health = 50.0f;
        hero.add_ability(Ability("looksmaxx", "Looksmaxx", DAMAGE, SINGLE, 10, -1));
        hero.add_ability(Ability("aura", "Aura", DAMAGE, AOE, 15, 2, 1));
        hero.add_ability(Ability("axe_slash",    "Axe Slash",    DAMAGE, SINGLE, 30, 1, 2, false));
        heroes.emplace_back(hero);
    }
    {
        Hero hero = Hero("Captain America");
        hero.health = hero.max_health = 100.0f;
        hero.add_ability(Ability("shield_throw", "Shield Throw", DAMAGE, SINGLE, 10));
        hero.add_ability(Ability("shield_bounce", "Shield Bounce", DAMAGE, AOE, 10, -1, 1));
        hero.add_ability(Ability("assemble", "Avengers... Assemble", HEAL, AOE, 20, -1, 2, false));
        heroes.emplace_back(hero);
    }
    {
        Hero hero = Hero("Homer Simpson");
        hero.health = hero.max_health = 40.0f;
        hero.add_ability(Ability("strangle", "Strangle", DAMAGE, SINGLE, 15, -1, 1));
        hero.add_ability(Ability("burp", "Burp", DAMAGE, AOE, 2));
        hero.add_ability(Ability("beer", "Duff Beer", HEAL, SELF, 10));
        heroes.emplace_back(hero);
    }
    {
        Hero hero = Hero("Joe Biden");
        hero.health = hero.max_health = 35.0f;
        hero.add_ability(Ability("sleepy_joe", "Sleepy Joe", HEAL, SELF, 20, 1, 0, false));
        hero.add_ability(Ability("stutter", "Stutter", DAMAGE, AOE, 3));
        heroes.emplace_back(hero);
    }
    {
        Hero hero = Hero("Gymskin");
        hero.health = hero.max_health = 70.0f;
        hero.add_ability(Ability("burnt_bean", "Burnt Bean", DAMAGE, SINGLE, 12));
        hero.add_ability(Ability("drop_shoulder", "Drop Shoulders", HEAL, SELF, 10));
        hero.add_ability(Ability("into_the_groove", "Into The Groove", DAMAGE, AOE, 7));
        heroes.emplace_back(hero);
    }
    {
        Hero hero = Hero("Thanos");
        hero.health = hero.max_health = 300.0f;
        hero.add_ability(Ability("infinity_punch", "Infinity Punch", DAMAGE, SINGLE, 20));
        hero.add_ability(Ability("power", "Power Stone", DAMAGE, AOE, 40, 1, 1, false));
        hero.add_ability(Ability("time", "Time Stone", HEAL, AOE, 35, 1, 2, false));
        heroes.emplace_back(hero);
    }

    return heroes;

}

std::unordered_map<std::string, Enemy> init_enemies() {
    std::unordered_map<std::string, Enemy> enemies;

    {
        Enemy e = Enemy("Slime Ball");
        e.max_health = 40.0f;
        e.health = 40.0f;
        e.add_ability(Ability("tackle", "Tackle", DAMAGE, SINGLE, 8));
        enemies.emplace("slime_ball", e);
    }
    {
        Enemy e = Enemy("Goblin");
        e.max_health = 60.0f;
        e.health = 60.0f;
        e.add_ability(Ability("stab", "Stab", DAMAGE, SINGLE, 15));
        e.add_ability(Ability("war_cry", "War Cry", DAMAGE, AOE, 8, 1)); // 1 use per battle
        enemies.emplace("goblin", e);
    }
    {
        Enemy e = Enemy("Wolf");
        e.max_health = 50.0f;
        e.health = 50.0f;
        e.add_ability(Ability("bite", "Bite", DAMAGE, SINGLE, 18));
        e.add_ability(Ability("howl_heal", "Alpha Howl", HEAL, SELF, 10, -1, 2)); // unlimited, 2 turn cooldown
        enemies.emplace("wolf", e);
    }
    {
        Enemy e = Enemy("Skeleton");
        e.health = e.max_health = 60.0f;
        e.add_ability(Ability("bone", "Bone", DAMAGE, SINGLE, 8));
        e.add_ability(Ability("bang_shield", "Shield Bang", HEAL, SELF, 10));
        enemies.emplace("skeleton", e);
    }
    {
        Enemy e = Enemy("Evil Spongebob");
        e.health = e.max_health = 30.0f;
        e.add_ability(Ability("laugh", "Laugh", DAMAGE, SINGLE, 6));
        e.add_ability(Ability("promotion", "Promotion", DAMAGE, SINGLE, 8));
        e.add_ability(Ability("goofy_goobers", "Goofy Goober", HEAL, SELF, 10));
        enemies.emplace("evil_spongebob", e);
    }
    {
        Enemy e = Enemy("Impostor");
        e.health = e.max_health = 100.0f;
        e.add_ability(Ability("sus", "Sus", DAMAGE, SINGLE, 15));
        e.add_ability(Ability("self_report", "Self Report", HEAL, SELF, 30, 1, 2, false));
        e.add_ability(Ability("tremble", "Trembling Voice", DAMAGE, SELF, 20, 1, 0, false));
        enemies.emplace("impostor", e);
    }

    return enemies;
}

std::vector<World> init_worlds(const std::unordered_map<std::string, Enemy>& enemies) {
    std::vector<World> worlds;

    {
        // create the first world "Humble Beginnings"
        World world1 = World("Humble Beginnings");

        // Create level 1-1
        Level level1_1 = Level(1, 1);
        level1_1.intro = "You find yourself in a slimy swamp and alls is fine";

        // Level 1-1 | First Battle
        {
            Battle battle;
            battle.intro = "After walking through the muddy swamps you realize... it's moving...";
            battle.enemies.push_back(enemies.at("slime_ball"));
            battle.enemies.push_back(enemies.at("slime_ball"));
            level1_1.add_battle(battle);
        }

        world1.add_level(level1_1);

        // Create level 1-2
        Level level1_2 = Level(1, 2);
        level1_2.intro = "That was close... no it wasn't. This might actually be too easy for you... then you hear a howl in the distance...";

        {
            Battle battle;
            battle.intro = "You look up to the sky and it is indeed a full moon.";
            battle.enemies.push_back(enemies.at("slime_ball"));
            battle.enemies.push_back(enemies.at("wolf"));
            level1_2.add_battle(battle);
        }
        {
            Battle battle;
            battle.intro = "This wasn't that bad, was it? \"What is that sound?\", you ask yourself... is that a...?";
            battle.enemies.push_back(enemies.at("skeleton"));
            battle.enemies.push_back(enemies.at("wolf"));
            battle.enemies.push_back(enemies.at("slime_ball"));

            level1_2.add_battle(battle);
        }

        world1.add_level(level1_2);

        // Create level 1-3
        Level level1_3 = Level(1, 3);
        level1_3.intro = "You have exited the swamp. It was so slimy... so muddy... so scary.";

        {
            Battle battle;
            battle.intro = "Okay... slime... wolf... what's next? Goblins?!";
            battle.enemies.push_back(enemies.at("goblin"));
            battle.enemies.push_back(enemies.at("goblin"));
            battle.enemies.push_back(enemies.at("goblin"));
            level1_3.add_battle(battle);
        }
        {
            Battle battle;
            battle.intro = "Can you hear it? \"yayayayaya\". What even is that? \"I... am EVIL SPONGEBOB\". OH NO!";
            battle.enemies.push_back(enemies.at("skeleton"));
            battle.enemies.push_back(enemies.at("evil_spongebob"));
            battle.enemies.push_back(enemies.at("skeleton"));
            level1_3.add_battle(battle);
        }

        world1.add_level(level1_3);

        Level level1_4 = Level(1, 4);
        level1_4.intro = "I must be in a dream. Tackling slime, goblins, skeletons... evil... spongebob... we must stay focused brothers!";

        {
            Battle battle;
            battle.intro = "You find yourself on Skeld. Do you know what that means???";
            battle.enemies.push_back(enemies.at("impostor"));
            level1_4.add_battle(battle);
        }

        world1.add_level(level1_4);

        worlds.emplace_back(world1);
    }

    return worlds;
}

}
