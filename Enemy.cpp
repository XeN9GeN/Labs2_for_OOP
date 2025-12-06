#include "Enemy.h"
#include "Character.h"
#include <string>
#include "Card.h"
#include <random>
#include <algorithm>

Enemy::Enemy(std::string name, int health, int armor, int mana) : name(name), Character(health, armor, mana) {}


void Enemy::autoEnemyTurn(Character& player) {
    drawCard();

    std::cout << "[ENEMY] " << name << " turn\n";

    int currentMana = getMana();
	auto& enemy_hand = getHand();

    if(enemy_hand.empty()) {
        int base_dmg = 2;
        std::cout << "[ENEMY] " << name << " performs basic attack for " << base_dmg << " dmg\n";
        player.getAttacked(base_dmg);
        return;
    }

    bool playedSmth = false;
    while (true) {
        int bestind = -1;
        int bestcost = -1;

        for (int i = 0; i < enemy_hand.size(); ++i) {
            auto& c = enemy_hand[i];
            if (!c) continue;

            int cost = c->getManaCost();
            if (cost <= getMana() && cost > bestcost) {
                bestcost = cost;
                bestind = i;
            }
        }
        if (bestind == -1) break;

        auto card = enemy_hand[bestind];
        std::cout << "[ENEMY] " << name << " uses: " << card->getName()
            << " (cost " << card->getManaCost() << ")\n";

        card->apply(*this, player); // self = enemy, target = player
        currentMana -= card->getManaCost();
        setMana(currentMana);

        enemy_hand.erase(enemy_hand.begin() + bestind);
        playedSmth = true;

        if (player.getHealth() <= 0) break;
    }

    if (!playedSmth) {
        int baseDmg = 2;
        std::cout << "[ENEMY] " << name << " performs basic attack for " << baseDmg << " dmg\n";
        player.getAttacked(baseDmg);
    }

   
}