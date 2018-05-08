// Yuxi Luo (yl4217), February 26, 2018
// CS3113 Game Programming

#include "GameState.hpp"
#include "../setUp.hpp"

extern ShaderProgram textured, untextured, lighting;

extern float fixedStep, screenWidth, screenHeight;
extern int maxStep;
extern glm::vec3 center;

enum GameMode {STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER};
extern GameMode mode;

GameState::GameState(): tile("Asset/tilemap", "Asset/level_1", 0.5), xml("Asset/sheet.xml"), level(1){
    untextured = setUntextured();
    center = tile.getPos();

    GLuint text;
    textured = setTextured("Asset/font1", text);
    disp = Text(&textured, text);

    // GLuint texture;
    GLuint texture;
    textured = setTextured("Asset/sheet", texture);
    lighting = setLight("Asset/sheet", texture);

    std::vector<XMLData> p;
    // 0: original; 1: jump; 2: stand; 3: walk1; 4: walk2
    p.push_back(xml.getData("alienBlue.png"));
    p.push_back(xml.getData("alienBlue_jump.png"));
    p.push_back(xml.getData("alienBlue_stand.png"));
    p.push_back(xml.getData("alienBlue_walk1.png"));
    p.push_back(xml.getData("alienBlue_walk2.png"));
    player1 = Player(&lighting, texture, p, center, &tile);
    player1.setScale(0.5);

    p.clear();
    p.push_back(xml.getData("alienYellow.png"));
    p.push_back(xml.getData("alienYellow_jump.png"));
    p.push_back(xml.getData("alienYellow_stand.png"));
    p.push_back(xml.getData("alienYellow_walk1.png"));
    p.push_back(xml.getData("alienYellow_walk2.png"));
    player2 = Player(&lighting, texture, p, center, &tile);
    player2.setScale(0.5);

    p.clear();
    p.push_back(xml.getData("alienBeige.png"));
    p.push_back(xml.getData("alienBeige_jump.png"));
    p.push_back(xml.getData("alienBeige_stand.png"));
    p.push_back(xml.getData("alienBeige_walk1.png"));
    p.push_back(xml.getData("alienBeige_walk2.png"));
    enemygroup = EnemyGroup(&lighting, texture, p, center, &tile);
    enemygroup.setScale(0.5);


    play1 = Object(&textured, texture, glm::vec3(-screenWidth + 1, screenHeight - 1 , 0));
    play1.setData(xml.getData("alienBlue.png"));
    play1.setScale(0.6);
    play1.update();

    play2 = Object(&textured, texture, glm::vec3(-screenWidth + 1, screenHeight - 1.6, 0));
    play2.setData(xml.getData("alienYellow.png"));
    play2.setScale(0.6);
    play2.update();
}

void GameState::init(){
    player1.setPos(center);
    player1.setVelo(0, 0);
    player2.setPos(center);
    player2.setVelo(0, 0);
    enemygroup.setPos(center);
    enemygroup.setVelo(0, 0);
}

float GameState::mapValue(float value, float srcMin, float srcMax, float dstMin, float dstMax) {
    float retVal = dstMin + ((value - srcMin)/(srcMax-srcMin) * (dstMax-dstMin));

    if(retVal < dstMin) {
        retVal = dstMin;
    }

    if(retVal > dstMax) {
        retVal = dstMax;
    }
    return retVal;
}


// bullets: disappear when collide
void GameState::checkCollision(float elapsed){
    // scale projection matrix
    glm::vec3 player1Pos = player1.getPos();
    glm::vec3 player2Pos = player2.getPos();
    float dist = sqrt(pow(player1Pos.x - player2Pos.x, 2) + pow(player1Pos.y - player2Pos.y, 2));
    dist = mapValue(dist, 0.0, 2 * screenWidth, 0.0, 2);
    if (dist < 0.5) dist = 0.5;

    tile.setProject(dist);
    player1.setProject(dist);
    player2.setProject(dist);
    enemygroup.setProject(dist);

    player1.satCollide(elapsed, enemygroup, player2);
    player2.satCollide(elapsed, enemygroup, player1);
    enemygroup.satCollide(elapsed);
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

void GameState::updateLevel(){
    if (player1.end || player2.end){

        // when reaching the end first: earn points
        if (player1.end) player1.points += 30;
        else player2.points += 30;

        player1.end = false;
        player2.end = false;

        // update to next level
        level += 1;
        if (level > 3) {
            mode = STATE_GAME_OVER;
            return;
        }
        tile.loadMap("Asset/level_" + std::to_string(level));
        tile.loadType("Asset/level_" + std::to_string(level));
        this->init();
    }
}


void GameState::update(float elapsed){
    switch (mode){
        case STATE_MAIN_MENU:
            break;
        case STATE_GAME_LEVEL:
            updateLevel();
            checkCollision(elapsed);
            break;
        case STATE_GAME_OVER:
            break;
    }
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

void GameState::displayData(){
    if (level <= 3) disp.renderLeft("Game Level: " + std::to_string(level) + "/3", 0.5, 0.6, -screenWidth + 0.8, screenHeight - 0.4);

    play1.setPos(-screenWidth + 1, screenHeight - 1);
    play1.update();
    play1.render();
    disp.renderLeft("Points: " + std::to_string(player1.points), 0.5, 0.6, -screenWidth + 1.5, screenHeight - 1);

    play2.setPos(-screenWidth + 1, screenHeight - 1.6);
    play2.update();
    play2.render();
    disp.renderLeft("Points: " + std::to_string(player2.points), 0.5, 0.6, -screenWidth + 1.5, screenHeight - 1.6);
}


void GameState::displayMainMenu(){
    disp.render("Racing", 1, 2, 0, 3.5);

    play1.setPos(-2.5, 0);
    play1.update();
    play1.render();
    disp.render("< ^ > to move", 0.5, 1, 0, 0);

    play2.setPos(-2.5, 1);
    play2.update();
    play2.render();
    disp.render("AWD to move", 0.5, 1, 0, 1);

    disp.render("B: begin   Q: quit", 0.5, 1, 0, -1.5);
}

void GameState::displayLevel(){
    // center the camera on the midpoint of player1 & player2
    Matrix viewMatrix;
    glm::vec3 player1Pos = player1.getPos();
    glm::vec3 player2Pos = player2.getPos();
    glm::vec3 midPos = (player1Pos + player2Pos) / float(2);
    viewMatrix.Translate(-midPos.x, -midPos.y, 0);

    // render light
    GLint lightPositionsUniform = glGetUniformLocation(lighting.programID, "lightPositions");
    GLfloat lightPositions[4];
    lightPositions[0] = player1Pos.x;
    lightPositions[1] = player1Pos.y;
    lightPositions[2] = player2Pos.x;
    lightPositions[3] = player2Pos.y;
    glUseProgram(lighting.programID);
    glUniform2fv(lightPositionsUniform, 2, lightPositions);

    // render tile first
    tile.render(viewMatrix);
    player1.render(viewMatrix);
    player2.render(viewMatrix);
    enemygroup.render(viewMatrix);

    displayData();
}

void GameState::displayOver(){
    disp.render("Game Over", 1, 2, 0, 1.5);

    if (player1.points > player2.points) {
        play1.setPos(0, 0);
        play1.setScale(1);
        play1.update();
        play1.render();
        disp.render("wins",  0.5, 1, 0, -1);
    } else if (player1.points > player2.points) {
        play2.setPos(0, 0);
        play2.setScale(1);
        play2.update();
        play2.render();
        disp.render("wins",  0.5, 1, 0, -1);
    } else {
        disp.render("Game ends with a draw",  0.5, 1, 0, -1);
    }

    displayData();
}
