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

GameState::GameState(): tile("Asset/tilemap"), xml("Asset/sheet.xml"){
    untextured = setUntextured();

    GLuint text;
    textured = setTextured("Asset/font1.png", text);
    disp = Text(&textured, text);

    init();
}

void GameState::init(){
    GLuint texture;
    textured = setTextured("Asset/sheet.png", texture);


    player = Player(texture, xml.getData("alienBlue.png"), tile.center());
    test = Player(texture, xml.getData("alienBeige.png"), glm::vec3(-2, 4, 0));
}


// bullets: disappear when collide
void GameState::checkCollision(float elapsed){
    player.collide(elapsed, test);
}

void GameState::update(float elapsed){
    switch (mode){
        case STATE_MAIN_MENU:
            break;
        case STATE_GAME_LEVEL:
            checkCollision(elapsed);
            test.update(elapsed);
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
    Matrix viewMatrix;
    glm::vec3 playerPos = player.center();
    viewMatrix.Translate(-playerPos.x, -playerPos.y, 0);

    player.render(viewMatrix);
    test.render(viewMatrix);
    tile.render(viewMatrix);

}

void GameState::displayOver(){
    disp.render("Game Over", 1, 2, 0, 1.5);
}
