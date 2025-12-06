#include <iostream>
#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
#include <ctime>

#include "Character.h"
#include "Enemy.h"
#include "Card.h"
#include "Game.h"

using namespace std;

int main() {
    Character player(30, 5, 10); // HP, Armor, Mana
    Enemy enemy("Armored Goblin", 20, 10, 10);
    Game game(player, enemy);
    enemy.addCardToDeck(make_shared<Attack_card>("Slash", "Simple strike", 2, 3));
    enemy.addCardToDeck(make_shared<Attack_card>("Heavy Blow", "Hard hit", 4, 5));
    enemy.addCardToDeck(make_shared<Defense_card>("Shield", "Block", 2, 2));
    enemy.addCardToDeck(make_shared<Heal_card>("Bandage", "Heal small", 3, 3));
    enemy.addCardToDeck(make_shared<Buff_card>("Rage", "Random buff", 1));
    enemy.addCardToDeck(make_shared<Debuff_card>("Curse", "Random debuff", 1));

    player.addCardToDeck(make_shared<Attack_card>("Strike", "Basic attack", 2, 4));
    player.addCardToDeck(make_shared<Defense_card>("Block", "Increase armor", 2, 3));
    player.addCardToDeck(make_shared<Heal_card>("First Aid", "Restore health", 3, 5));
    player.addCardToDeck(make_shared<Buff_card>("Inspiration", "Random buff", 1));
    player.addCardToDeck(make_shared<Debuff_card>("Hex", "Random debuff", 1));
    player.addCardToDeck(make_shared<Debuff_card>("Deadly Venom", "Applies poison", 1, StatusType::Poison, 4));
    player.addCardToDeck(make_shared<Debuff_card>("Bleeding Strike", "Causes bleeding", 1, StatusType::Bleed, 3));
    player.addCardToDeck(make_shared<Debuff_card>("Intimidate", "Weakens enemy", 1, StatusType::Weak, 2));

    player.drawCard();
    player.drawCard();
    player.drawCard();
    player.drawCard();

    enemy.drawCard();
    enemy.drawCard();
    enemy.drawCard();
    enemy.drawCard();

    bool auto_mode = false;
    cout << "Choose mode:\n";
    cout << "1 - Auto battle\n";
    cout << "2 - Manual battle (you play your turn)\n";
    cout << "> ";

    int choice;
    cin >> choice;

    if (choice == 1) {
        auto_mode = true;
        cout << "\n[MODE] Auto battle enabled.\n";
    }
    else {
        auto_mode = false;
        cout << "\n[MODE] Manual mode enabled.\n";
    }




    std::cout << "Battle Start!" << endl;

    int turn = 1;
    while (player.getHealth() > 0 && enemy.getHealth() > 0) {
        std::cout << "\n\n\n\n";
        std::cout << "\n--- Turn " << turn << " ---\n";

        player.setMana(turn);
        enemy.setMana(turn);

        if (auto_mode) {
            game.autoPlayerTurn();      // игрок ходит автоматически
        }
        else {
            game.PlayerTurn();          // игрок вводит команды сам
        }
        enemy.autoEnemyTurn(player);    // враг ВСЕГДА ходит автоматически


        player.printCaracterStatus(player, "Player");
        enemy.printEnemyStatus(enemy,"Enemy: ");

        turn++;
    }
}


