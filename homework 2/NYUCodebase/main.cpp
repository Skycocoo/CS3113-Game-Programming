// Yuxi Luo (yl4217), February 15, 2018
// Homework 2, PONG!, CS3113 Game Programming

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <cstdlib>
#include <vector>

// define Object as an entity in this game
// header files for shaderprogram & matrix & sdl are included
#include "Object.h"
#include "setUp.h"

using namespace std;
float screenRatio;
float screenHeight;
float screenWidth;
float splitScale;

int playerScore = 0;
int enemyScore = 0;


// calculate positions in advance to reduce redundancy
// return the size of tiles for collision detection
void splitInit(const Object& obj, vector<Matrix>& splitPos, int num){
    // position for middle split line
    splitScale = float(2) / float(num);
    for(int i = 0; i < num; i++){
        float relative = (i - float(num-1) / float(2)) * 2 * screenHeight / float(num);
        
        Matrix pos;
        pos.Translate(0, relative, 0);
        pos.Scale(splitScale, splitScale, splitScale);
        
        splitPos.push_back(pos);
    }
    
    // position for upper & lower boundaries
    int numBoundary = 50;
    for (int i = 0; i < numBoundary; i++){
        float relative = (i - float(numBoundary) / float(2)) * 2 * screenWidth / float(numBoundary);
        Matrix posH;
        posH.Translate(relative, screenHeight - splitScale / 2, 0);
        posH.Scale(splitScale, splitScale, splitScale);
        
        splitPos.push_back(posH);
        
        Matrix posL;
        posL.Translate(relative, -screenHeight + splitScale / 2, 0);
        posL.Scale(splitScale, splitScale, splitScale);
        
        splitPos.push_back(posL);
    }
}


// draw the middle line to split the window into two parts
void drawSplit(Object& obj, const vector<Matrix>& splitPos){
    for(int i = 0; i < splitPos.size(); i++){
        obj.modelMatrix = splitPos[i];
        obj.display();
    }
}

// detect collision:
//    if collide: with upper/lower boundary : velocity y = - velocity y
//    with bars: verlocity x = - velocity x
//    https://stackoverflow.com/questions/10808929/2d-collision-detection-for-pong
gi
void collisionDetection(Object& obj, const vector<Object*>& bars){
    bool collide = false;
    
    float   objUp = obj.y + obj.height / 2,
    objDown = obj.y - obj.height / 2,
    objLeft = obj.x - obj.width / 2,
    objRight = obj.x + obj.width / 2;
    
    for(size_t i = 0; i < bars.size(); i++){
        float enUp = bars[i]->y + bars[i]->height / 2,
        enDown = bars[i]->y - bars[i]->height / 2,
        enLeft = bars[i]->x - bars[i]->width / 2,
        enRight = bars[i]->x + bars[i]->width / 2;
        
        // intersecting
        if (!(objUp < enDown || objLeft > enRight || objDown > enUp || objRight < enLeft)){
            if ((obj.velocity_x > 0 && bars[i]->x > 0) || (obj.velocity_x < 0 && bars[i]->x < 0)){
                collide = true;
            }
        }
    }
    
    if (collide) obj.velocity_x = -obj.velocity_x;
    
    // upper / lower boundary collision
    if ((objUp > screenHeight - splitScale / 2) || (objDown < -screenHeight + splitScale / 2)) {
        obj.velocity_y = -obj.velocity_y;
    }
}

void initiatePong(Object& pong){
    pong.x = 0;
    pong.y = 0;
    pong.velocity_x = ((rand()) % 4 - 2) * 3;
    pong.velocity_y = ((rand()) % 4 - 2) * 3;
    while (pong.velocity_x == 0) pong.velocity_x = ((rand()) % 4 - 2) * 3;
    while (pong.velocity_y == 0) pong.velocity_y = ((rand()) % 4 - 2) * 3;
}

void updatePong(Object& pong, const vector<Object*>& bars, float elapsed){
    
    pong.x += elapsed * pong.velocity_x;
    pong.y += elapsed * pong.velocity_y;
    
    collisionDetection(pong, bars);
    
    pong.modelMatrix.Identity();
    pong.modelMatrix.Translate(pong.x, pong.y, 0);
    
    if (pong.x - pong.width / 2 < -screenWidth) {
        playerScore += 1;
        initiatePong(pong);
    } else if (pong.x + pong.width / 2 > screenWidth) {
        enemyScore += 1;
        initiatePong(pong);
    }
}

// update the slide bar to be automatically moved by time
void updateSlide(Object& obj, float elapsed, bool& up){
    float distance = elapsed * obj.velocity_y;

    if (obj.y > screenHeight - distance - obj.height / 2 - splitScale / 2) up = false;
    else if (obj.y < -screenHeight + distance + obj.height / 2 + splitScale / 2) up = true;
    if (up) obj.y += distance;
    else obj.y -= distance;

    obj.modelMatrix.Identity();
    obj.modelMatrix.Translate(obj.x, obj.y, 0);
}

void updatePlayer(Object& obj){
    obj.modelMatrix.Identity();
    obj.modelMatrix.Translate(obj.x, obj.y, 0);
}


void reGame(Object& pong, vector<Object*>& bars) {
    initiatePong(pong);
    playerScore = 0;
    enemyScore = 0;
    
    for (size_t i = 0; i < bars.size(); i++){
        bars[i]->y = 0;
    }
}

// if one side scores >= 10 then win (display the result & end the game)
bool win() {return (playerScore >= 10 || enemyScore >= 10);}

// display the score for each side in the middle of the game
void displayGame(Object& disp){
    disp.text("player: " + to_string(playerScore), 0.8, 1, 2, screenHeight - 1);
    disp.text("enemy: " + to_string(enemyScore), 0.8, 1, -5, screenHeight - 1);
    
    if (win()) {
        glClear(GL_COLOR_BUFFER_BIT);
        
        string winner = (playerScore >= 10) ? "player" : "enemy";
        disp.text(winner + "wins", 0.8, 1, -1.5, screenHeight - 1);
        disp.text("press R to restart game",  0.8, 1, -4, screenHeight - 3);
        disp.text("press Q to end the game",  0.8, 1, -4, screenHeight - 5);
    }
}


int main(){
    // initial set up
    srand(time(NULL));
    SDL_Window* displayWindow = setUp("Homework 2");
    
    // setting up texts
    GLuint texture1;
    ShaderProgram text = setTextured("font1.png", texture1);
    Object disp(text, 0, 0, 0, 0, 0, 0, true, texture1);
    
    // setting up objects
    ShaderProgram prog = setUntextured();
    
    // split line
    Object split(prog, false);
    vector<Matrix> splitPos;
    splitInit(split, splitPos, 20);

    // player & enemy (computer control)
    vector<Object*> bars;
    bool enemyUp = true;
    Object player(prog, 5, 0, 0.2, 2);
    Object enemy(prog, -5, 0, 0.2, 2, 0, 3);
    bars.push_back(&player);
    bars.push_back(&enemy);
    
    // ping pong
    Object pong(prog, 0, 0, 0.2, 0.2, false);
    initiatePong(pong);

    // game loop
    float lastFrameTicks = 0.0f;
    SDL_Event event;
    bool done = false, restart = false, regame = false;
    while (!done) {
        
        
        // keyboard event
        while (SDL_PollEvent(&event)) checkKeyboard(event, done, restart, regame, player);
        
        if (restart){
            initiatePong(pong);
            restart = false;
        }
        
        if (regame){
            reGame(pong, bars);
            regame = false;
        }
        
        // update parameters
        float ticks = (float)SDL_GetTicks()/1000.0f;
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        
        updateSlide(enemy, elapsed, enemyUp);
        updatePong(pong, bars, elapsed);
        updatePlayer(player);
        
        // display
        glClear(GL_COLOR_BUFFER_BIT);
        
        displayGame(disp);
        drawSplit(split, splitPos);
        enemy.display();
        player.display();
        pong.display();
        
        SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}
