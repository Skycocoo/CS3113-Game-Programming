// Yuxi Luo (yl4217), February 15, 2018
// Homework 2, PONG!, CS3113 Game Programming

// To do:
// update constructor of Object
// clear up main()
// determine the winning condition
// change the movement of the player bar

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

// update the slide bar to be automatically moved by time
void updateSlide(Object& obj, float elapsed, bool& up){
    float distance = elapsed * obj.velocity_y;

    if (obj.y > screenHeight - distance - obj.width / 2 - splitScale / 2) up = false;
    else if (obj.y < -screenHeight + distance + obj.width / 2 + splitScale / 2) up = true;
    if (up) obj.y += distance;
    else obj.y -= distance;

    obj.modelMatrix.Identity();
    obj.modelMatrix.Translate(obj.x, obj.y, 0);
}


// direction of Pong based on the current scorer?
void initiatePong(Object& obj){
    obj.velocity_x = (rand()) % 10 + 1;
    obj.velocity_y = (rand()) % 5 + 1;
}


// detect collision
//    if collide: with upper/lower boundary : velocity y = - velocity y
//    with bars: verlocity x = - velocity x
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
            collide = true;
        }
        
        // if (i == 0) cout << objUp << " " << enDown << " " << objLeft << " " << enRight << " " << objDown << " " << enUp << " " << objRight << " " << enLeft << endl;
    }
    
    if (collide) obj.velocity_x = -obj.velocity_x;
    
    // upper / lower boundary collision
    if ((objUp > screenHeight - splitScale / 2) || (objDown < -screenHeight + splitScale / 2)) {
        obj.velocity_y = -obj.velocity_y;
    }
    
}

void updatePong(Object& obj, const vector<Object*>& bars, float elapsed){
    collisionDetection(obj, bars);
    obj.x += elapsed * obj.velocity_x;
    obj.y += elapsed * obj.velocity_y;
    
    obj.modelMatrix.Identity();
    obj.modelMatrix.Translate(obj.x, obj.y, 0);
    
    if (obj.x - obj.width / 2 < -screenWidth) playerScore += 1;
    else if (obj.x + obj.width / 2 > screenWidth) enemyScore += 1;
    // need to restart the game
}


void dispalyGame(){
//    display the player & computer's position (before the start of the game)
//    display the score for each side in the middle of the game
//    display the sign of an object which Pong will point towards (player or enemy)
    
    
    
}


void win(){
    //    determine the end of game
    //    if one side scores >= 10 then win (display the result & end the game)
    //    if not: continue the game
    
}

int main(){
    // initial set up
    srand(time(NULL));
    SDL_Window* displayWindow = setUp("Homework 2");
    
    // setting up objects
    ShaderProgram prog = setUntextured();
    
    // split line
    Object split(prog, false);
    vector<Matrix> splitPos;
    splitInit(split, splitPos, 20);
    
    
    // player & enemy (computer control)
    vector<Object*> bars;
    
//    bool playerUp = true;
    Object player(prog, false);
    player.x = 5;
    player.velocity_y = 3;
    player.modelMatrix.Translate(player.x, player.y, 0);
    
    bool enemyUp = true;
    Object enemy(prog, false);
    enemy.x = -5;
    enemy.velocity_y = 3;
    
    bars.push_back(&player);
    bars.push_back(&enemy);
    
    // ping pong
    Object pong(prog, false);
    initiatePong(pong);

    // game loop
    float lastFrameTicks = 0.0f;
    SDL_Event event;
    bool done = false;
    while (!done) {
        
        // keyboard event
        while (SDL_PollEvent(&event)) checkKeyboard(event, done, player);
        
        // update parameters
        float ticks = (float)SDL_GetTicks()/1000.0f;
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        
        updateSlide(enemy, elapsed, enemyUp);
//        updateSlide(player, elapsed, playerUp);
        updatePong(pong, bars, elapsed);
        
        // display
        glClear(GL_COLOR_BUFFER_BIT);
        
        drawSplit(split, splitPos);
        enemy.display();
        player.display();
        pong.display();
        
        SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}
