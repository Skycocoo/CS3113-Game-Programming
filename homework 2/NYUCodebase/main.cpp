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
void initiatePong(Object& pong){
    srand(time(NULL));
    
    pong.velocity_x = (rand()) % 10 - 5;
    pong.velocity_y = (rand()) % 10 - 5;
}


// detect collision
//    if collide: with upper/lower boundary : velocity y = - velocity y
//    with bars: verlocity x = - velocity x
void collisionDetection(Object& obj, const vector<Object*>& bars){
    bool collidex = false, collidey = false;
    

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
            float lengthx = 0, lengthy = 0;
            
            if (objLeft < enLeft) lengthx = (objRight < enRight) ? objRight - enLeft : enRight - enLeft;
            else lengthx = (objRight < enRight) ? objRight - objLeft : enRight - objLeft;
            
            if (objUp > enUp) lengthy = (objDown > enDown) ? enUp - objDown : enUp - enDown;
            else lengthy = (objDown > enDown) ? objUp - objDown : objUp - enDown;
            
            if (lengthx > lengthy) collidey = true;
            else if (lengthx < lengthy) collidex = true;
            else {
                collidey = true;
                collidex = true;
            }
            
//            if ((obj.height / 2 + bars[i]->height / 2) < (obj.y - bars[i]->y) < (sqrt(2) * obj.height / 2 + sqrt(2) * bars[i]->height / 2)) collidey = true;
//            if ((obj.width / 2 + bars[i]->width / 2) < (obj.x - bars[i]->x) < (sqrt(2) * obj.width / 2 + sqrt(2) * bars[i]->width / 2)) collidex = true;
//
//            if (objUp > enDown || objDown < enUp) collidey = true;
//            if (objLeft < enRight || objRight > enLeft) collidex = true;
            
//            if (!(objUp < enDown || objDown > enUp)) collidey = true;
//            else if (!(objLeft > enRight || objRight < enLeft)) collidex = true;
        }
        
        // if (i == 0) cout << objUp << " " << enDown << " " << objLeft << " " << enRight << " " << objDown << " " << enUp << " " << objRight << " " << enLeft << endl;
    }
    
    if (collidex) obj.velocity_x = -obj.velocity_x;
    if (collidey) obj.velocity_y = -obj.velocity_y;
    
    // upper / lower boundary collision
    if ((objUp > screenHeight - splitScale / 2) || (objDown < -screenHeight + splitScale / 2)) {
        obj.velocity_y = -obj.velocity_y;
    }
    
}

void newRound(Object& pong){
    pong.x = 0;
    pong.y = 0;
    initiatePong(pong);
}

void updatePong(Object& pong, const vector<Object*>& bars, float elapsed){
    collisionDetection(pong, bars);
    pong.x += elapsed * pong.velocity_x;
    pong.y += elapsed * pong.velocity_y;
    
    pong.modelMatrix.Identity();
    pong.modelMatrix.Translate(pong.x, pong.y, 0);
    
    if (pong.x - pong.width / 2 < -screenWidth) {
        playerScore += 1;
        newRound(pong);
    } else if (pong.x + pong.width / 2 > screenWidth) {
        enemyScore += 1;
        newRound(pong);
    }
    
}



//    display the score for each side in the middle of the game
void displayGame(Object& disp){
//    disp.display();
    disp.text("hafdafwei", 0.5, 0.0001);
}


void win(){
    //    determine the end of game
    //    if one side scores >= 10 then win (display the result & end the game)
    //    if not: continue the game
    
}

int main(){
    // initial set up
    
    SDL_Window* displayWindow = setUp("Homework 2");
    
    // setting up texts
    GLuint texture1;
    ShaderProgram text = setTextured("font1.png", texture1);
    Object disp(text, true, texture1);
    
    
    
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
    bool restart = false;
    while (!done) {
        
        
        // keyboard event
        while (SDL_PollEvent(&event)) checkKeyboard(event, done, restart, player);
        
        if (restart){
            newRound(pong);
            restart = false;
        }
        
        // update parameters
        float ticks = (float)SDL_GetTicks()/1000.0f;
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        
        updateSlide(enemy, elapsed, enemyUp);
//        updateSlide(player, elapsed, playerUp);
        updatePong(pong, bars, elapsed);
        
        
        
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
