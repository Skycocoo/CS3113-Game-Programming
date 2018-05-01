// Yuxi Luo (yl4217), February 26, 2018
// CS3113 Game Programming

#ifndef GameState_hpp
#define GameState_hpp

#include "../Objects/Player.hpp"
#include "../Objects/Enemy.hpp"
#include "../Objects/Text.hpp"
#include "../Objects/Tile.hpp"
#include "XMLLoad.hpp"


class GameState{
public:
    Player player1;
    Player player2;
    EnemyGroup enemygroup;

    Tile tile;

    GameState();
    void init();

    void checkCollision(float elapsed);
    void update(float elapsed);
    void fixedUpdate(float lastFrameTicks, float accumulator);
    void render();

    void displayMainMenu();
    void displayLevel();
    void displayOver();

private:
    XMLLoad xml;
    Text disp;

    float mapValue(float value, float srcMin, float srcMax, float dstMin, float dstMax);
    float easeIn(float from, float to, float time);
};

#endif /* GameState_hpp */
