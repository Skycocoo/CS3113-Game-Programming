// Yuxi Luo (yl4217), February 26, 2018
// CS3113 Game Programming

#ifndef GameState_h
#define GameState_h

#include "Player.h"
#include "Enemy.h"
#include "Text.h"

#include "XMLLoad.h"

class GameState{
public:
    // public fields
    XMLLoad xml;
    Text disp;

    Player player;
    EnemyGroup enemygroup;

    GLuint texture;

    // methods
    GameState();
    virtual void init() = 0;

    void checkCollision();
    void update(float elapsed);
    void fixedUpdate(float lastFrameTicks, float accumulator);
    void render();

    virtual void displayMainMenu();
    virtual void displayLevel();
    virtual void displayOver();


};


#endif /* GameState_h */
