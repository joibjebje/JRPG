#include "ui/tui.h"
#include "core/hero.h"
#include "core/game.h"
#include "util/util.h"
#include "world/world.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>
#include <algorithm>

namespace jrpg {

    void TUI::clear_screen() {

        std::cout << "\033[2J\033[H";

    }

    void TUI::print_separator(int width) {

        std::cout << std::string(width, '-') << "\n";

    }

    void TUI::print_header(const std::string& title, int width) {

        print_separator(width);
        int pad = (width - static_cast<int>(title.size())) / 2;
        std::cout << std::string(std::max(0, pad), ' ') << title << "\n";
        print_separator(width);
        std::cout << "Enter 'q', 'quit', 'exit' at any time to quit\n";

    }

    void TUI::wait_for_enter() {

        std::cout << "\n[Press Enter to continue]";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    }

    bool TUI::check_exit(const std::string& choice_str) {

        return
            choice_str == "q" ||
            choice_str == "quit" ||
            choice_str == "exit";
            // add more options here if needed

    }

    std::string TUI::user_prompt() {

        std::cout << "> ";
        std::string input;
        std::cin >> input;

        // check if the user has entered an exit string
        if (check_exit(input)) {
            exit(0);  // ungraceful quit
        }

        return input;

    }

    std::string TUI::user_prompt_line() {

        std::cout << "> ";
        std::string input;
        std::getline(std::cin, input);

        if (check_exit(input)) {
            exit(0);  // ungraceful quit
        }

        return input;

    }

    // Function that handles user input when options are provided
    int TUI::prompt_choice(const std::string& prompt, const std::vector<std::string>& options) {

        std::cout << "\n" << prompt << "\n";
        for (int i = 0; i < static_cast<int>(options.size()); i++) {
            std::cout << " " << (i + 1) << ") " << options[i] << "\n";
        }
        while (true) {
            std::string input = user_prompt();
            if (input == "b" || input == "back") return -1;

            int choice = atoi(input.c_str());
            if (choice >= 1 && choice <= static_cast<int>(options.size())) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return choice - 1;
            }

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid choice.\n";
        }

    }

    /*
            MAIN MENU
    */
    int TUI::show_main_menu() {

        clear_screen();
        print_header("JRPG");

        return prompt_choice("What would you like to do?", {
            "Play", "Quit"
        });

    }


    /*
            PLAY MENU
    */
    int TUI::show_play_menu() {

        clear_screen();
        print_header("Play");
        std::cout << "Enter 'b' or 'back' to go back.\n";

        return prompt_choice("What would you like to do?", {
            "Worlds",
            "Heroes",
        });

    }

    /*
            WORLDS MENU
    */
    int TUI::show_worlds_menu(Game* game) {

        clear_screen();
        print_header("Worlds");
        std::cout << "Enter 'b' or 'back' to go back.\n";

        if (game->worlds.empty()) {
            std::cout << "\nNo worlds available.\n";
            wait_for_enter();
            return -1;
        }

        std::vector<std::string> options;
        for (auto& w : game->worlds) {
            options.push_back(w.name);
        }

        return prompt_choice("Select a world:", options);

    }

    /*
            LEVELS MENU
    */
    int TUI::show_levels_menu(Game* game, int world_idx) {

        World& world = game->worlds.at(world_idx);

        clear_screen();
        print_header(world.name);
        std::cout << "Enter 'b' or 'back' to go back.\n";

        if (world.level_count() == 0) {
            std::cout << "\nNo levels in this world.\n";
            wait_for_enter();
            return -1;
        }

        std::vector<std::string> options;
        for (int i = 0; i < world.level_count(); i++) {
            Level& lvl = world.get_level(i);
            std::string label = "Level " + std::to_string(lvl.world_idx) + "-" + std::to_string(lvl.level_idx);
            label += " (" + std::to_string(lvl.battle_count()) + " battle" + (lvl.battle_count() != 1 ? "s" : "") + ")";
            options.push_back(label);
        }

        return prompt_choice("Select a level:", options);

    }

    /*
            HEROES MENU
    */
    bool TUI::show_heroes_menu(Game* game, bool confirm_mode) {

        clear_screen();
        print_header(confirm_mode ? "Select Your Party" : "Heroes");

        if (confirm_mode) {
            std::cout << "Enter 'c' or 'confirm' to start (need at least 1 in party).\n";
        } else {
            std::cout << "Enter 'b' or 'back' to go back.\n";
        }

        // d x to show hero details
        // a x to assign/unassign hero
        std::cout << "\n";
        std::cout << std::setw(4) << "- Assign/Unassign = 'a x' or 'assign x'\n";
        std::cout << std::setw(4) << "- Hero Details    = 'd x' or 'details x'\n";

        if (confirm_mode) {
            std::cout << "\n  Party size: " << game->party_size() << "\n";
        }

        int longest_name = 0;
        for (Hero& h : game->heroes) {
            longest_name = std::max(longest_name, static_cast<int>(h.name.length()));
        }

        int i = 1;
        for (Hero& h : game->heroes) {

            std::cout << std::setw(3) << i++ << ": "
                      << std::left << std::setw(longest_name + 2) << h.name
                      << std::right << "| " << std::setw(5) << (h.inParty ? "PARTY" : "") << " |\n";

        }

        while (true) {
            std::string input = user_prompt_line();

            if (input.empty()) continue;

            // Back (only in normal mode)
            if (!confirm_mode && (input == "b" || input == "back")) {
                return false;
            }

            // Confirm (only in confirm mode)
            if (confirm_mode && (input == "c" || input == "confirm")) {
                if (game->party_size() >= 1) {
                    return false;  // close the menu, party is confirmed
                }
                std::cerr << "You need at least 1 hero in your party!\n";
                continue;
            }

            auto validate_hero_idx = [](std::string& part2) {

                // isNumber found in util.h
                if (isNumber(part2)) {
                    return std::atoi(part2.c_str());
                }
                return -1;

            };

            std::vector<std::string> parts = split(input, ' ');
            if (parts[0] == "a" || parts[0] == "assign") {

                if (parts.size() != 2) {

                    std::cerr << "Usage: (a|assign) x\n";
                    continue;

                }

                int hero_idx = validate_hero_idx(parts[1]);
                if (hero_idx > 0 && hero_idx <= static_cast<int>(game->heroes.size())) {

                    if (!game->assign_hero(hero_idx - 1)) {
                        std::cerr << "Party is full! (max " << Game::MAX_PARTY_SIZE << " heroes). Unassign one first.\n";
                        continue;
                    }
                    return true;

                } else {
                    std::cerr << "Invalid Hero Index (Range: 1.." << game->heroes.size() << ")\n";
                    continue;
                }


            }

            if (parts[0] == "d" || parts[0] == "details") {

                if (parts.size() != 2) {
                    std::cerr << "Usage: (d|details) x\n";
                    continue;
                }

                int hero_idx = validate_hero_idx(parts[1]);
                if (hero_idx > 0 && hero_idx <= static_cast<int>(game->heroes.size())) {

                    hero_details(game->heroes.at(hero_idx-1));
                    return true;

                } else {
                    std::cerr << "Invalid Hero Index (Range: 1.." << game->heroes.size() << ")\n";
                    continue;
                }

            }

            std::cerr << "Unknown command '" << input << "'.\n";
        }

    }

    void TUI::hero_details(Hero hero) {

        clear_screen();
        print_header(hero.name);

        std::cout << "\n";
        std::cout << "  HP:       " << hero.health << " / " << hero.max_health << "\n";
        std::cout << "  Status:   " << (hero.isDead ? "Dead" : "Alive") << "\n";
        std::cout << "  In Party: " << (hero.inParty ? "Yes" : "No") << "\n";

        std::cout << "\n  Abilities:\n";
        const auto& abilities = hero.get_abilities();
        if (abilities.empty()) {
            std::cout << "    (none)\n";
        } else {
            for (const auto& a : abilities) {
                std::cout << "    - " << a.name
                          << " [" << a.type_str() << "/" << a.target_str()
                          << ", Power: " << a.power;
                if (a.max_uses != -1) {
                    std::cout << ", Uses: " << a.max_uses;
                }
                if (a.cooldown > 0) {
                    std::cout << ", CD: " << a.cooldown << " turns";
                }
                if (!a.charged_at_start) {
                    std::cout << ", Uncharged";
                }
                std::cout << "]\n";
            }
        }

        wait_for_enter();
    }


} // namespace jrpg
