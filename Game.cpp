#include "Game.h"
#include <iostream>
#include <memory>
#include "Card.h"
#include "Character.h"
#include "Enemy.h"

Game::Game(Character& p, Enemy& e) : player(p), enemy(e){}


void Game::logPlay(const std::string& entry) {
    play_log.push_back(entry);
}

void Game::printPlayLog() const {
    if (play_log.empty()) return;
    std::cout << "[LOG] Played this turn:\n";
    for (const auto& e : play_log) {
        std::cout << " - " << e << "\n";
    }
}


void Game::PlayerTurn() {
        std::cout << "Player turn...\n";
        
		player.printCaracterStatus(player, "Player");
        enemy.printEnemyStatus(enemy, "Enemy: ");

		auto& hand = player.getHand();
		int currentMana = player.getMana();
        
        if (hand.empty()) { std::cout << "Hand is empty"; }
        
        while (true) {
            std::cout << "-------------------------\n";
            std::cout << "Mana: " << currentMana << "\n";
            std::cout << "Hand:\n";

            for (int i = 0; i < hand.size(); ++i) {
                if (hand[i]) {
                    std::cout << " [" << i << "] ";
                    hand[i]->printCard();
                }
                else {
                    std::cout << " [" << i << "] <empty>\n";
                }
            }

            std::cout << "\nChoose action:\n";
            std::cout << " p <index>  - Play card with index\n";
			std::cout << " r           - Draw card\n";
            std::cout << " a          - Basic attack\n";
            std::cout << " e          - End turn\n";
            std::cout << "> ";

            std::string cmd;
            std::cin >> cmd;
            if(cmd=="p"){
                int indx;
				std::cin >> indx;
                auto card = hand[indx];
                int cost = card->getManaCost();
                if (cost > currentMana) { std::cout << "Not Enough Mana!!!"; continue; }


                std::cout << "Player use card: " << card->getName();
                logPlay(card->getName());
                card->apply(player, enemy);//ПЕРЕПРОВЕРИТЬ ДЛЯ ТИПА BUFF
                currentMana -= cost;
                player.setMana(currentMana);
                hand.erase(hand.begin() + indx);
       
                std::cout << "-------------------------\n";
                printPlayLog();
            }
            else if (cmd == "r") {
                player.drawCard();
				std::cout << "Player draws a card.\n";
            }
            else if (cmd == "a") {
                int dmg = 2;
                enemy.getAttacked(dmg);
                std::cout << "Игрок наносит базовую атаку: " << dmg << " урона\n";
                logPlay(std::string("Basic attack: ") + std::to_string(dmg) + " dmg");

                std::cout << "-------------------------\n";
                printPlayLog();

                // После атаки считаем ход завершённым — выходим из цикла
                break;
            }

            else if (cmd == "e") {
                // Завершить ход без дополнительных действий
                break;
            }
            else {
                std::cout << "Неизвестная команда.\n";
            }
            std::cout << "\n";
        }
}
void Game::autoPlayerTurn() {
    std::cout << "[AUTO] Player turn...\n";

    int currentMana = player.getMana();
    auto& enemy_hand = player.getHand();

    int bestIndex = -1;
    int bestCost = -1;

    // найти лучшую карту в руке
    for (int i = 0; i < enemy_hand.size(); ++i) {
        auto& c = enemy_hand[i];
        if (!c) continue;
        int cost = c->getManaCost();
        if (cost <= currentMana && cost > bestCost) {
            bestCost = cost;
            bestIndex = i;
        }
    }
    play_log.clear();

    if (bestIndex != -1) {
        auto card = enemy_hand[bestIndex];
        std::cout << "[AUTO] Player uses card: " << card->getName() << "\n";
        logPlay(card->getName());
        card->apply(player, enemy);
        player.setMana(currentMana - card->getManaCost());
        enemy_hand.erase(enemy_hand.begin() + bestIndex);
        std::cout << "-------------------------\n";
        printPlayLog();
        return;
    }

    int dmg = 2;
    enemy.getAttacked(dmg);
    std::cout << "[AUTO] Player attacks for " << dmg << " damage\n";
    logPlay(std::string("Basic attack: ") + std::to_string(dmg) + " dmg");

    std::cout << "-------------------------\n";
    printPlayLog();
    std::cout << "\n\n\n";

}