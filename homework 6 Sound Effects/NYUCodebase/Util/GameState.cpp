// Yuxi Luo (yl4217), February 26, 2018
// CS3113 Game Programming

#include "GameState.hpp"
#include "../setUp.hpp"

extern ShaderProgram untextured;
extern ShaderProgram textured;

enum GameMode {STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER};
extern GameMode mode;
extern float fixedStep;
extern int maxStep;

extern glm::vec3 center;

GameState::GameState(): tile("Asset/tilemap"), xml("Asset/sheet.xml"){
    untextured = setUntextured();
    center = tile.getCenter();

    GLuint text;
    textured = setTextured("Asset/font1.png", text);
    disp = Text(&textured, text);

    // GLuint texture;
    GLuint texture;
    textured = setTextured("Asset/sheet.png", texture);

    player = Player(texture, xml.getData("alienBlue.png"), center, &tile);
    player.setScale(0.5);

    enemygroup = EnemyGroup(texture, xml.getData("alienBeige.png"), center, &tile);
}

void GameState::init(){
    player.setPos(center);
    enemygroup.setPos(center);
}


// bullets: disappear when collide
void GameState::checkCollision(float elapsed){
    // player.collide(elapsed, enemygroup);
    // enemygroup.collide(elapsed);
    player.satTwoCollide(elapsed, enemygroup.ene[0], enemygroup.ene[1]);
    enemygroup.ene[0].satTwoCollide(elapsed, player, enemygroup.ene[1]);
    enemygroup.ene[1].satTwoCollide(elapsed, player, enemygroup.ene[0]);
}

void GameState::update(float elapsed){
    switch (mode){
        case STATE_MAIN_MENU:
            break;
        case STATE_GAME_LEVEL:
            checkCollision(elapsed);
            break;
        case STATE_GAME_OVER:
            break;
    }
}

void GameState::fixedUpdate(float lastFrameTicks, float accumulator){
    // update parameters
    float ticks = (float)SDL_GetTicks()/1000.0f;
    float elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;

    // get elapsed time
    elapsed += accumulator;
    if (elapsed < fixedStep) {
        accumulator = elapsed;
        return;
    }

    int count = 0;
    while (elapsed >= fixedStep) {
        update(fixedStep);
        elapsed -= fixedStep;
        count += 1;

        if (count >= maxStep) break;
    }
    accumulator = elapsed;
}


void GameState::render(){
    switch (mode){
        case STATE_MAIN_MENU:
            displayMainMenu();
            break;
        case STATE_GAME_LEVEL:
            displayLevel();
            break;
        case STATE_GAME_OVER:
            displayOver();
            break;
    }

}


void GameState::displayMainMenu(){
    disp.render("Platformer", 1, 2, 0, 3.5);
    disp.render("<=   =>   to move", 0.5, 1, 0, 1);

    disp.render("B: begin   Q: quit", 0.5, 1, 0, -1.5);
}

void GameState::displayLevel(){
    // center the camera on player
    Matrix viewMatrix;
    glm::vec3 playerPos = player.getCenter();
    viewMatrix.Translate(-playerPos.x, -playerPos.y, 0);

    player.render(viewMatrix);
    enemygroup.render(viewMatrix);
    tile.render(viewMatrix);
}

void GameState::displayOver(){
    disp.render("Game Over", 1, 2, 0, 1.5);
}
