#pragma once
#include <string>
#include "Character.h"
#include "Enemy.h"

class Game {
private:
    Character &player;
    Enemy &enemy;
    std::vector<std::string> play_log; // журнал сыгранных карт / действий

public:
    Game(Character&p,Enemy&e);
    void PlayerTurn();
    void autoPlayerTurn();

    void logPlay(const std::string& entry);
    void printPlayLog() const;
};

