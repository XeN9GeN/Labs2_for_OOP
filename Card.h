#pragma once
#include <string>
#include <iostream>
#include "Character.h"
using namespace std;

enum class CardType {
	Attack,
	Defense,
	Heal,
	Buff,
	Debuff
};
enum class StatusType {
	Poison,
	Bleed,
	Weak,
	Vulnerable,
	Fragile
};


class Card
{
protected: 
	string name;
	string effect_d;
	int mana_cost;
	CardType type;
	Card(string n, string e_d, int m_c, CardType t) : name(n), effect_d(e_d), mana_cost(m_c), type(t) {}

public:

	virtual ~Card() = default;
	virtual void apply(Character& self, Character& target) = 0;
	virtual void printCard() = 0;

	const string& getName() const { return name; }
	const string& getEffectDescription() const { return effect_d; }
	int getManaCost() const { return mana_cost; }
	CardType getType() const { return type; }
};



class Attack_card : public Card {
	int dmg;
public:
	Attack_card(string n, string e_d, int m_c, int type_value) : Card(n, e_d, m_c, CardType::Attack), dmg(type_value) {}
	void apply(Character& self, Character& target) override;
	void printCard() override;
};


class Defense_card : public Card {
	int armor;
public:
	Defense_card(string n, string e_d, int m_c, int type_value) : Card(n, e_d, m_c, CardType::Defense), armor(type_value) {}
	void apply(Character& self, Character& target) override;
	void printCard() override;
};


class Heal_card : public Card {
	int heal_amount;
public:
	Heal_card(string n, string e_d, int m_c, int type_value) : Card(n, e_d, m_c, CardType::Heal), heal_amount(type_value) {}
	void apply(Character& self, Character& target) override;
	void printCard() override;
};


class Buff_card : public Card {
	// Buff specific attributes
public:
	Buff_card(string n, string e_d, int m_c) : Card(n, e_d, m_c, CardType::Buff) {}
	void apply(Character& self, Character& target) override;
	void printCard() override;
};


class Debuff_card : public Card {
	StatusType statusType;
	int value; // сила статуса (например, урон от яда, кровотечения)
public:
	// Добавлены значения по умолчанию, чтобы можно было вызывать и с 3 аргументами
	Debuff_card(string n, string e_d, int m_c, StatusType st = StatusType::Poison, int val = 1) : Card(n, e_d, m_c, CardType::Debuff), statusType(st), value(val) {}
	void apply(Character& self, Character& target) override;
	void printCard() override;
};
