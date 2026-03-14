#pragma once

#include <string>
#include <vector>

#include "core/hero.h"
#include "core/game.h"

namespace jrpg {

class TUI {
public:
    static void clear_screen();
    static void print_separator(int width = 60);
    static void print_header(const std::string& title, int width = 60);
    static void wait_for_enter();
    static std::string user_prompt();
    static std::string user_prompt_line();
    static int prompt_choice(
        const std::string& prompt,
        const std::vector<std::string>& options);

    static bool check_exit(const std::string& choice_str);

    // Main Menu
    static int show_main_menu();

    // Play Menu
    static int show_play_menu();

    // Worlds Menu - returns selected world index, or -1 for back
    static int show_worlds_menu(Game* game);

    // Levels Menu - returns selected level index, or -1 for back
    static int show_levels_menu(Game* game, int world_idx);

    // Heroes Menu
    // confirm_mode: if true, shows "confirm" instead of "back"
    //               and requires party_size >= 1 to confirm
    // returns true to refresh the screen, false to close
    static bool show_heroes_menu(Game* game, bool confirm_mode = false);

    static void hero_details(Hero hero);
};

}
