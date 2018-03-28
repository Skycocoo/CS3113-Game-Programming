// Yuxi Luo (yl4217), February 26, 2018
// CS3113 Game Programming

#ifndef GameState_h
#define GameState_h

#include "../Objects/Player.h"
#include "../Objects/Enemy.h"
#include "../Objects/Text.h"
#include "XMLLoad.h"


class GameState{
public:
    Player player;
    EnemyGroup enemygroup;

    GameState();
    void init();

    void checkCollision();
    void update(float elapsed);
    void fixedUpdate(float lastFrameTicks, float accumulator);
    void render();

    void displayMainMenu();
    void displayLevel();
    void displayOver();

private:
    XMLLoad xml;
    Text disp;
};


#endif /* GameState_h */
