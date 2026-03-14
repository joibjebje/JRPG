#include "battle/battle.h"
#include "ui/tui.h"

#include <iostream>
#include <iomanip>
#include <limits>
#include <vector>
#include <cstdlib>
#include <ctime>

namespace jrpg {

static void print_battle_status(std::vector<Hero*>& party, std::vector<Enemy>& enemies) {

    std::cout << "\n";
    std::cout << "  --- Party ---\n";
    for (Hero* h : party) {
        if (h->isDead) {
            std::cout << "    " << h->name << "  [DEAD]\n";
        } else {
            std::cout << "    " << h->name << "  HP: " << h->health << "/" << h->max_health << "\n";
        }
    }

    std::cout << "\n  --- Enemies ---\n";
    int i = 1;
    for (Enemy& e : enemies) {
        if (e.isDead) {
            std::cout << "    " << i++ << ". " << e.name << "  [DEAD]\n";
        } else {
            std::cout << "    " << i++ << ". " << e.name << "  HP: " << e.health << "/" << e.max_health << "\n";
        }
    }
    std::cout << "\n";

}

bool Battle::run(std::vector<Hero*>& party) {

    static bool seeded = false;
    if (!seeded) { std::srand(static_cast<unsigned>(std::time(nullptr))); seeded = true; }

    // Reset enemies and abilities for this battle
    for (Enemy& e : enemies) {
        e.health = e.max_health;
        e.isDead = false;
        e.reset_abilities();
    }
    for (Hero* h : party) {
        h->reset_abilities();
    }

    auto check_victory = [&]() -> bool {
        for (Enemy& e : enemies) {
            if (!e.isDead) return false;
        }
        return true;
    };

    auto check_defeat = [&]() -> bool {
        for (Hero* h : party) {
            if (!h->isDead) return false;
        }
        return true;
    };

    int turn = 1;

    while (true) {

        // Tick cooldowns at the start of each turn (except turn 1)
        if (turn > 1) {
            for (Hero* h : party) {
                h->tick_cooldowns();
            }
            for (Enemy& e : enemies) {
                e.tick_cooldowns();
            }
        }

        // Player Turn
        TUI::clear_screen();
        TUI::print_header("Battle - Turn " + std::to_string(turn));
        print_battle_status(party, enemies);

        for (Hero* hero : party) {
            if (hero->isDead) continue;

            std::cout << "  " << hero->name << "'s turn:\n";

            auto& abilities = hero->get_abilities_mut();

            // Show abilities with availability
            bool has_available = false;
            for (int i = 0; i < static_cast<int>(abilities.size()); i++) {
                Ability& a = abilities[i];
                std::string status = a.status_str();
                bool avail = a.is_available();
                if (avail) has_available = true;

                std::cout << "    " << (i + 1) << ") " << a.name
                          << " [" << a.type_str() << "/" << a.target_str()
                          << ", Pow: " << a.power;

                // Show uses if limited
                if (a.max_uses != -1) {
                    std::cout << ", Uses: " << a.uses_left << "/" << a.max_uses;
                }
                // Show cooldown info if it has one
                if (a.cooldown > 0) {
                    std::cout << ", CD: " << a.cooldown << "t";
                }

                std::cout << "] ";

                if (!avail) {
                    std::cout << "(" << status << ")";
                }
                std::cout << "\n";
            }

            if (!has_available) {
                std::cout << "  No abilities available! Skipping turn.\n\n";
                continue;
            }

            // Pick ability
            int ability_idx = -1;
            while (true) {
                std::string input = TUI::user_prompt();
                ability_idx = std::atoi(input.c_str()) - 1;
                if (ability_idx >= 0 && ability_idx < static_cast<int>(abilities.size())) {
                    if (abilities[ability_idx].is_available()) {
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        break;
                    }
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "  That ability is not available! (" << abilities[ability_idx].status_str() << ")\n";
                    continue;
                }
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "  Invalid choice.\n";
            }

            Ability& ability = abilities[ability_idx];

            // Pick target(s) based on ability
            std::vector<Entity*> targets;

            if (ability.type == DAMAGE) {

                if (ability.target == SINGLE) {
                    std::cout << "  Choose target:\n";
                    int ei = 1;
                    for (Enemy& e : enemies) {
                        if (!e.isDead) {
                            std::cout << "    " << ei << ") " << e.name
                                      << " (HP: " << e.health << ")\n";
                        }
                        ei++;
                    }
                    while (true) {
                        std::string input = TUI::user_prompt();
                        int tidx = std::atoi(input.c_str()) - 1;
                        if (tidx >= 0 && tidx < static_cast<int>(enemies.size()) && !enemies[tidx].isDead) {
                            targets.push_back(&enemies[tidx]);
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            break;
                        }
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "  Invalid target.\n";
                    }
                } else if (ability.target == AOE) {
                    for (Enemy& e : enemies) {
                        if (!e.isDead) targets.push_back(&e);
                    }
                } else if (ability.target == SELF) {
                    // Self-damage, no targets needed - execute handles it via caster
                }

            } else if (ability.type == HEAL) {

                if (ability.target == SELF) {
                    // No target needed
                } else if (ability.target == SINGLE) {
                    std::cout << "  Choose ally to heal:\n";
                    int ai = 1;
                    for (Hero* h : party) {
                        if (!h->isDead) {
                            std::cout << "    " << ai << ") " << h->name
                                      << " (HP: " << h->health << ")\n";
                        }
                        ai++;
                    }
                    while (true) {
                        std::string input = TUI::user_prompt();
                        int tidx = std::atoi(input.c_str()) - 1;
                        if (tidx >= 0 && tidx < static_cast<int>(party.size()) && !party[tidx]->isDead) {
                            targets.push_back(party[tidx]);
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            break;
                        }
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "  Invalid target.\n";
                    }
                } else if (ability.target == AOE) {
                    for (Hero* h : party) {
                        if (!h->isDead) targets.push_back(h);
                    }
                }

            }

            ability.execute(*hero, targets);
            std::cout << "\n";

            // Check victory after each hero action
            if (check_victory()) {
                std::cout << "\n  Victory!\n";
                TUI::wait_for_enter();
                return true;
            }

        }

        // Enemy Turn
        std::cout << "  --- Enemy Turn ---\n\n";

        for (Enemy& enemy : enemies) {
            if (enemy.isDead) continue;

            // Collect alive heroes
            std::vector<Hero*> alive_heroes;
            for (Hero* h : party) {
                if (!h->isDead) alive_heroes.push_back(h);
            }
            if (alive_heroes.empty()) break;

            // Pick a random available ability
            auto& e_abilities = enemy.get_abilities_mut();
            if (e_abilities.empty()) continue;

            // Collect available abilities
            std::vector<int> available;
            for (int i = 0; i < static_cast<int>(e_abilities.size()); i++) {
                if (e_abilities[i].is_available()) available.push_back(i);
            }
            if (available.empty()) continue;  // skip turn if nothing available

            int chosen = available[std::rand() % available.size()];
            Ability& ability = e_abilities[chosen];

            std::vector<Entity*> targets;

            if (ability.type == DAMAGE) {
                if (ability.target == SINGLE) {
                    Hero* target = alive_heroes[std::rand() % alive_heroes.size()];
                    targets.push_back(target);
                } else if (ability.target == AOE) {
                    for (Hero* h : alive_heroes) {
                        targets.push_back(h);
                    }
                } else if (ability.target == SELF) {
                    // Self-damage, no targets needed - execute handles it via caster
                }
            } else if (ability.type == HEAL) {
                if (ability.target == SELF) {
                    // No targets needed
                } else {
                    std::vector<Enemy*> alive_enemies;
                    for (Enemy& e : enemies) {
                        if (!e.isDead) alive_enemies.push_back(&e);
                    }
                    if (ability.target == SINGLE && !alive_enemies.empty()) {
                        targets.push_back(alive_enemies[std::rand() % alive_enemies.size()]);
                    } else {
                        for (Enemy* e : alive_enemies) targets.push_back(e);
                    }
                }
            }

            ability.execute(enemy, targets);

            // Check defeat after each enemy action
            if (check_defeat()) {
                std::cout << "\n  Defeat...\n";
                TUI::wait_for_enter();
                return false;
            }

        }

        TUI::wait_for_enter();
        turn++;

    }

}

}
