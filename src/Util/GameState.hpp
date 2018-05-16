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
    Tile background;

    GameState();
    void init();
    void cheat();

    void initLevel();
    void easeInLevel();
    void updateLevel();

    void checkCollision(float elapsed);

    void update(float elapsed);
    void fixedUpdate(float lastFrameTicks, float accumulator);
    void render();

    void displayMainMenu();
    void displayLevel();
    void displayOver();
    void displayData();

private:
    XMLLoad xml;
    Text disp;

    Object play1;
    Object play2;

    int level;

    std::chrono::system_clock::time_point start;
    float fadeInTime = 2.0;
    bool initial = false;
    bool changeToTexture = false;
    bool changeToLight = false;

    float mapValue(float value, float srcMin, float srcMax, float dstMin, float dstMax);
};

#endif /* GameState_hpp */
