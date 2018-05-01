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

GameState::GameState(): tile("Asset/tilemap", "Asset/level_2", 0.5), xml("Asset/sheet.xml"){
    untextured = setUntextured();
    center = tile.getCenter();

    GLuint text;
    textured = setTextured("Asset/font1", text);
    disp = Text(&textured, text);

    // GLuint texture;
    GLuint texture;
    textured = setTextured("Asset/sheet", texture);


    std::vector<XMLData> p;
    // 0: original; 1: jump; 2: stand; 3: walk1; 4: walk2
    p.push_back(xml.getData("alienBlue.png"));
    p.push_back(xml.getData("alienBlue_jump.png"));
    p.push_back(xml.getData("alienBlue_stand.png"));
    p.push_back(xml.getData("alienBlue_walk1.png"));
    p.push_back(xml.getData("alienBlue_walk2.png"));
    player = Player(texture, p, center, &tile);
    player.setScale(0.5);

    p.clear();
    p.push_back(xml.getData("alienBeige.png"));
    p.push_back(xml.getData("alienBeige_jump.png"));
    p.push_back(xml.getData("alienBeige_stand.png"));
    p.push_back(xml.getData("alienBeige_walk1.png"));
    p.push_back(xml.getData("alienBeige_walk2.png"));
    enemygroup = EnemyGroup(texture, p, center, &tile);
    enemygroup.setScale(0.5);
}

void GameState::init(){
    player.setPos(center);
    player.setVelo(0, 0);
    enemygroup.setPos(center);
    enemygroup.setVelo(0, 0);
}


// bullets: disappear when collide
void GameState::checkCollision(float elapsed){
    player.satCollide(elapsed, enemygroup);
    enemygroup.satCollide(elapsed);

    // player.satTwoCollide(elapsed, enemygroup.ene[0], enemygroup.ene[1]);
    // enemygroup.ene[0].satTwoCollide(elapsed, player, enemygroup.ene[1]);
    // enemygroup.ene[1].satTwoCollide(elapsed, player, enemygroup.ene[0]);
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
