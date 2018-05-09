// Yuxi Luo (yl4217), February 26, 2018
// CS3113 Game Programming

#include "GameState.hpp"
#include "../setUp.hpp"

extern ShaderProgram textured, untextured, lighting;

extern float fixedStep, screenWidth, screenHeight;
extern int maxStep;
extern glm::vec3 startPos;

enum GameMode {STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER};
extern GameMode mode;

GameState::GameState(): tile("Asset/tilemap", "Asset/level_1", 0.5), background("Asset/tilemap", "Asset/background", 0.5), xml("Asset/sheet.xml"), level(1){
    untextured = setUntextured();
    startPos = tile.getPos();

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
    player1 = Player(&lighting, texture, p, startPos, &tile);
    player1.setScale(0.5);

    p.clear();
    p.push_back(xml.getData("alienYellow.png"));
    p.push_back(xml.getData("alienYellow_jump.png"));
    p.push_back(xml.getData("alienYellow_stand.png"));
    p.push_back(xml.getData("alienYellow_walk1.png"));
    p.push_back(xml.getData("alienYellow_walk2.png"));
    player2 = Player(&lighting, texture, p, startPos, &tile);
    player2.setScale(0.5);

    p.clear();
    p.push_back(xml.getData("alienBeige.png"));
    p.push_back(xml.getData("alienBeige_jump.png"));
    p.push_back(xml.getData("alienBeige_stand.png"));
    p.push_back(xml.getData("alienBeige_walk1.png"));
    p.push_back(xml.getData("alienBeige_walk2.png"));
    enemygroup = EnemyGroup(&lighting, texture, p, startPos, &tile);
    enemygroup.setScale(0.5);


    play1 = Object(&textured, texture);
    play1.setData(xml.getData("alienBlue.png"));
    play1.setScale(0.6);
    play1.update();

    play2 = Object(&textured, texture);
    play2.setData(xml.getData("alienYellow.png"));
    play2.setScale(0.6);
    play2.update();

    init();
}

void GameState::init(){
    player1.setPos(startPos.x - 0.5, startPos.y);
    player1.setVelo(0, 0);
    // player2.setPos(startPos);
    player2.setPos(startPos.x + 0.5, startPos.y);
    player2.setVelo(0, 0);
    enemygroup.setPos(startPos);
    enemygroup.setVelo(0, 0);
}

void GameState::cheat(){
    level += 1;
    if (level > 3) {
        mode = STATE_GAME_OVER;
        return;
    }
    initLevel();
}


// bullets: disappear when collide
void GameState::checkCollision(float elapsed){
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


//////////////////////// handling levels ////////////////////////

void GameState::initLevel(){
    if (level != 1){
        tile.loadMap("Asset/level_" + std::to_string(level));
        tile.loadType("Asset/level_" + std::to_string(level));
        this->init();
    }
    start = std::chrono::system_clock::now();
    changeToLight = false;
    changeToTexture = false;
}

void GameState::easeInLevel(){
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double>  elapsed = end - start;
    if (elapsed.count() < fadeInTime){
        tile.easeIn(elapsed.count(), fadeInTime);
        // background.easeIn(elapsed.count(), fadeInTime);
    }
}

void GameState::updateLevel(){
    if (!initial){
        initLevel();
        initial = true;
        return;
    }

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
        initLevel();
    }
}


//////////////////////// handling game modes ////////////////////////

void GameState::update(float elapsed){
    switch (mode){
        case STATE_MAIN_MENU:
            break;
        case STATE_GAME_LEVEL:
            updateLevel();
            easeInLevel();
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

//////////////////////// handling display ////////////////////////

void GameState::displayMainMenu(){
    // render light
    GLint brightnessUniform = glGetUniformLocation(textured.programID, "brightness");
    glUseProgram(textured.programID);
    glUniform1f(brightnessUniform, 1);

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

    glm::vec3 player1Pos = player1.getPos();
    glm::vec3 player2Pos = player2.getPos();
    Matrix viewMatrix;

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double>  elapsed = end - start;

    if (elapsed.count() >= fadeInTime){
        if (!changeToLight){
            tile.setShader(&lighting);
            background.setShader(&lighting);
            changeToLight = true;
        }

        // render light
        GLint brightnessUniform = glGetUniformLocation(lighting.programID, "brightness");
        GLint lightPositionsUniform = glGetUniformLocation(lighting.programID, "lightPositions");
        GLfloat lightPositions[4];
        lightPositions[0] = player1Pos.x;
        lightPositions[1] = player1Pos.y;
        lightPositions[2] = player2Pos.x;
        lightPositions[3] = player2Pos.y;
        glUseProgram(lighting.programID);
        glUniform2fv(lightPositionsUniform, 2, lightPositions);

        // scale the projection based on the distance between player1 and player2
        float dist = sqrt(pow(player1Pos.x - player2Pos.x, 2) + pow(player1Pos.y - player2Pos.y, 2));
        dist = mapValue(dist, 0.0, 2 * screenWidth, 0, 2);
        if (dist < 0.5) dist = 0.5;

        tile.setProject(dist);
        background.setProject(dist);

        player1.setProject(dist);
        player2.setProject(dist);
        enemygroup.setProject(dist);

        // startPos the camera on the midpoint of player1 & player2
        glm::vec3 midPos = (player1Pos + player2Pos) / float(2);
        viewMatrix.Translate(-midPos.x, -midPos.y, 0);

        // render tile first
        glUniform1f(brightnessUniform, 0.5);
        background.render(viewMatrix);

        glUniform1f(brightnessUniform, 1);
        tile.render(viewMatrix);
        player1.render(viewMatrix);
        player2.render(viewMatrix);
        enemygroup.render(viewMatrix);

    } else {
        if (!changeToTexture){
            tile.setShader(&textured);
            background.setShader(&textured);
            changeToTexture = true;
        }

        float scale = 1 - elapsed.count() / fadeInTime;

        // render light
        GLint brightnessUniform = glGetUniformLocation(textured.programID, "brightness");

        // calculate the distance
        float width = (tile.map.mapWidth * tile.tilesize) / 2.0 - startPos.x;
        float height = (-tile.map.mapHeight * tile.tilesize) / 2.0 - startPos.y;
        float distWidth = width * scale;
        float distHeight = height * scale;

        distWidth = mapValue(distWidth, 0.0, width, startPos.x, (tile.map.mapWidth * tile.tilesize) / 2.0);
        distHeight = mapValue(distHeight, 0.0, height, startPos.y - 0.25, (-tile.map.mapHeight * tile.tilesize) / 2.0);

        // projection matrix: display the entire map
        tile.setProject(mapValue(scale, 0.0, 1.0, 0.5, 2.0));
        background.setProject(mapValue(scale, 0.0, 1.0, 0.5, 2.0));

        // view matrix: reverse of desired startPos position
        viewMatrix.Translate(-distWidth, -distHeight, 0);

        glUseProgram(textured.programID);
        glUniform1f(brightnessUniform, mapValue(scale, 0.0, 1.0, 0.5, 0.7));
        background.render(viewMatrix);
        glUniform1f(brightnessUniform, mapValue(scale, 0.0, 1.0, 0.6, 1.0));
        tile.render(viewMatrix);
    }

    displayData();
}

void GameState::displayOver(){
    // render light
    GLint brightnessUniform = glGetUniformLocation(textured.programID, "brightness");
    glUseProgram(textured.programID);
    glUniform1f(brightnessUniform, 1);

    disp.render("Game Over", 1, 2, 0, 1.5);

    if (player1.points > player2.points) {
        play1.setPos(0, 0);
        play1.setScale(1);
        play1.update();
        play1.render();
        disp.render("wins",  0.5, 1, 0, -1);
    } else if (player1.points < player2.points) {
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

void GameState::displayData(){
    // render light
    GLint brightnessUniform = glGetUniformLocation(textured.programID, "brightness");
    glUseProgram(textured.programID);
    glUniform1f(brightnessUniform, 1);

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

//////////////////////// helper function ////////////////////////

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
