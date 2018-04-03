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
    // textured = setTextured("Asset/sheet.png", texture);

    player = DynamicObj(0, center, &tile);
    player.setScale(0.64);
    player.setRotate(45 * M_PI / 180);

    ene = DynamicObj(0, glm::vec3(center.x+0.5, center.y+0.5, 0), &tile);

    ene2 = DynamicObj(0, glm::vec3(center.x-0.5, center.y-0.5, 0), &tile);
    ene2.setScale(1.2);
    ene2.setRotate(30 * M_PI / 180);
    // ene.setRotate(2);
    // enemygroup = EnemyGroup(texture, xml.getData("alienBeige.png"), center, &tile);

    init();
}

void GameState::init(){
    player.setPos(center);
    // for (size_t i = 0; i < ene.size(); i++) ene[i].setPos(center);
//    enemygroup.setPos(center);
}


// bullets: disappear when collide
void GameState::checkCollision(float elapsed){
   //  float scale = 1.0;
   //
   // glm::vec3 p = player.getVelo();
   // float scale = (p.y == 0) ? 1.0 : fabs(p.y);
   // if (!player.coll.bottom){
   //     if (scale > 3) scale = 1;
   //     else if (scale < 1) scale = 3;
   //     else scale = 3 / scale;
   //
   //     std::cout << p.y << " " << scale << std::endl;
   // }
   //
   //  player.setProject(scale);
   //  enemygroup.setProject(scale);
   //  tile.setProject(scale);


    // // std::cout << "\nenemy satcollide" << std::endl;
    // ene.satCollide(elapsed, player);
    // // ene.collide(elapsed);
    //
    // // std::cout << "\nplayer satcollide" << std::endl;
    // player.satCollide(elapsed, ene);

    player.satTwoCollide(elapsed, ene, ene2);
    ene.satTwoCollide(elapsed, player, ene2);
    ene2.satTwoCollide(elapsed, player, ene);

//    for (size_t i = 0; i < ene.size(); i++){
//        player.collide(elapsed, ene[i]);
//    }

//    ene.collide(elapsed);

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
    // for (size_t i = 0; i < ene.size(); i++){
    ene.render(viewMatrix);
    ene2.render(viewMatrix);
    // }

    tile.render(viewMatrix);

}

void GameState::displayOver(){
    disp.render("Game Over", 1, 2, 0, 1.5);
}
