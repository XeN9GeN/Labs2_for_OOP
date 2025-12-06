#include "Character.h"
#include "Card.h"
#include <iostream>
#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
#include <ctime>
#include "Enemy.h"


static std::mt19937 make_rng() {
    static std::random_device rd;
    return std::mt19937(static_cast<unsigned>(std::time(nullptr)) ^ rd());
}


void Character::applyPhysicalDamageToArmor(int dmg) {// проверка на ломание полного щита
    armor -= dmg;
    if (armor < 0) armor = 0;
} 
void Character::getAttacked(int dmg) {
	if (hp <= 0) return;

	int incoming_dmg = dmg;

	// Apply Vulnerable status    more dmg
    if(status.vulnerable > 0) {
        incoming_dmg = static_cast<int>(incoming_dmg * 1.5);
	}
    
	// Apply Weak status    defShred
	int effective_armor = armor;
    if(status.weak > 0) {
        effective_armor /= 2;
    }
     
	//Damage calculation after armor
	int dmg_after_armor = incoming_dmg - effective_armor;// дмг который должен пройти по хп
    if(dmg_after_armor < 0) {
        dmg_after_armor = 0;
	}

	applyPhysicalDamageToArmor(incoming_dmg);// Reduce armor first
	hp -= dmg_after_armor;// Apply damage to health
	if (hp < 0)  hp = 0;
   
}

void Character::showStatuses() const {
    cout << "Statuses:\n";

    if (status.poison > 0)
        cout << "- Poison: " << status.poison << "\n";

    if (status.bleed > 0)
        cout << "- Bleed: " << status.bleed << "\n";

    if (status.weak > 0)
        cout << "- Weak: " << status.weak << " turns\n";

    if (status.vulnerable > 0)
        cout << "- Vulnerable: " << status.vulnerable << " turns\n";

    if (status.fragile > 0)
        cout << "- Fragile: " << status.fragile << " turns\n";

    // Если статусов нет
    if (status.poison == 0 && status.bleed == 0 &&
        status.weak == 0 && status.vulnerable == 0 &&
        status.fragile == 0)
    {
        cout << "No statuses.\n";
    }
}


void Character::startTurn() {

    if (status.poison > 0) {
        hp -= status.poison;
        cout << "Poison deals " << status.poison << " damage!\n";
		if (hp < 0) hp = 0;
    }

    if (status.bleed > 0) {
        int effective_armor = armor;
        if (status.fragile > 0) { effective_armor /= 2; }

        int dmg_after_armor = status.bleed - effective_armor;
        if (dmg_after_armor < 0) { dmg_after_armor = 0; }
        
		applyPhysicalDamageToArmor(status.bleed);
        hp -= dmg_after_armor;
		if (hp < 0) hp = 0;

		cout << "Bleed deals " << dmg_after_armor << " damage after armor!\n";
    }

}
void Character::endTurn() {
    if (status.poison > 0) status.poison = max(0, status.poison - 1);
    if (status.bleed > 0) status.bleed = std::max(0, status.bleed - 1);
    if (status.weak > 0) status.weak = std::max(0, status.weak - 1);
    if (status.vulnerable > 0) status.vulnerable = std::max(0, status.vulnerable - 1);
    if (status.fragile > 0) status.fragile = std::max(0, status.fragile - 1);
}


void Character::drawCard() {
    if (player_deck.empty()) return;
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<size_t> dist(0, player_deck.size() - 1);
    size_t index = dist(rng);
    player_hand.push_back(player_deck[index]);
    player_deck.erase(player_deck.begin() + index);
}
