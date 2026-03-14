#include "core/game.h"
#include "core/init.h"
#include "ui/tui.h"

#include <iostream>
#include <string>

namespace jrpg {

    Game::Game() {}

    void Game::start() {

        // Init heroes, enemies, and worlds
        heroes = init_heroes();
        enemies = init_enemies();
        worlds = init_worlds(enemies);

        while (!shouldQuit) {
            int choice = TUI::show_main_menu();
            switch (choice) {
                case 0:
                    play();
                    break;
                case 1:
                    quit();
                    break;
            }
        }
        TUI::clear_screen();
        std::cout << "Come back soon!\n";

    }

    void Game::play() {

        while(true) {
            int choice = TUI::show_play_menu();

            // Go back
            if (choice == -1) break;

            switch(choice) {
                case 0: world_menu(); break;
                case 1: hero_menu(); break;
            }
        }

    }

    void Game::world_menu() {

        while (true) {
            int world_idx = TUI::show_worlds_menu(this);
            if (world_idx == -1) return;

            while (true) {
                int level_idx = TUI::show_levels_menu(this, world_idx);
                if (level_idx == -1) break;

                // Player selected a level - show hero selection in confirm mode
                while (true) {
                    if (!TUI::show_heroes_menu(this, true)) {
                        break;  // party confirmed (or would be back, but confirm_mode has no back)
                    }
                    // true = refresh the screen
                }

                // Check if we actually have a party
                if (party_size() >= 1) {
                    Level& level = worlds.at(world_idx).get_level(level_idx);
                    start_level(level);
                }
            }
        }

    }

    void Game::hero_menu() {

        while (true) {
            // if true, then refresh, else break the hero menu, as in go back
            if (!TUI::show_heroes_menu(this)) {
                break;
            }
        }

    }

    void Game::start_level(Level& level) {

        std::vector<Hero*> party_heroes = get_party();

        auto& battles = level.get_battles();

        std::string level_name = "Level " + std::to_string(level.world_idx) + "-" + std::to_string(level.level_idx);

        if (battles.empty()) {
            TUI::clear_screen();
            TUI::print_header(level_name);
            std::cout << "\nNo battles in this level.\n";
            TUI::wait_for_enter();
            return;
        }

        // Level intro screen
        if (!level.intro.empty()) {
            TUI::clear_screen();
            TUI::print_header(level_name);
            std::cout << "\n" << level.intro << "\n";
            TUI::wait_for_enter();
        }

        for (int i = 0; i < static_cast<int>(battles.size()); i++) {

            // Battle intro screen
            if (!battles[i].intro.empty()) {
                TUI::clear_screen();
                TUI::print_header(level_name + " - Battle " + std::to_string(i + 1));
                std::cout << "\n" << battles[i].intro << "\n";
                TUI::wait_for_enter();
            }

            bool won = battles[i].run(party_heroes);

            if (!won) {
                TUI::clear_screen();
                std::cout << "\nYour party was defeated...\n";
                TUI::wait_for_enter();

                // Revive heroes after defeat so the game can continue
                for (Hero* h : party_heroes) {
                    h->health = h->max_health;
                    h->isDead = false;
                }

                return;
            }

        }

        TUI::clear_screen();
        std::cout << "\nLevel " << level.world_idx << "-" << level.level_idx << " completed!\n";

        // Restore party HP after completing a level
        for (Hero* h : party_heroes) {
            h->health = h->max_health;
            h->isDead = false;
        }

        TUI::wait_for_enter();

    }

    void Game::quit() {

        shouldQuit = true;

    }

    bool Game::assign_hero(int hero_idx) {

        // check if this hero is already in party
        // if so, unassign it. Else assign
        if (heroes.at(hero_idx).inParty) {
            heroes.at(hero_idx).inParty = false;
            return true;
        }

        // Can't add more if party is full
        if (party_size() >= MAX_PARTY_SIZE) {
            return false;
        }

        heroes.at(hero_idx).inParty = true;
        return true;

    }

    int Game::party_size() const {

        int count = 0;
        for (const Hero& h : heroes) {
            if (h.inParty) count++;
        }
        return count;

    }

    std::vector<Hero*> Game::get_party() {

        std::vector<Hero*> result;
        for (Hero& h : heroes) {
            if (h.inParty) result.push_back(&h);
        }
        return result;

    }

}
