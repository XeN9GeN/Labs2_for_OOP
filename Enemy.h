#pragma once
#include <vector>
#include <memory>
#include "Character.h"
class Card;

class Enemy : public Character {
	std::string	name;
public:
	Enemy(std::string name, int health, int armor, int mana);
	void autoEnemyTurn(Character&player);
	void printEnemyStatus(const Enemy& enemy, const std::string& name) {
		std::cout << "-------------------------\n";
		std::cout << name << "- HP: " << enemy.getHealth() << ", Armor: " << enemy.getArmor() << ", Mana: " << enemy.getMana() << std::endl;
		enemy.showStatuses();
	}

};

