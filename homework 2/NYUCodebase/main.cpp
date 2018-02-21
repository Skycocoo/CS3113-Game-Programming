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

void updatePlayer(Object& obj){
    obj.modelMatrix.Identity();
    obj.modelMatrix.Translate(obj.x, obj.y, 0);
}


// direction of Pong based on the current scorer?
void initiatePong(Object& pong){
    
    
    while (pong.velocity_x == 0) pong.velocity_x = ((rand()) % 4 - 2) * 2;
    while (pong.velocity_y == 0) pong.velocity_y = ((rand()) % 4 - 2) * 2;
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
        
//         if (i == 0) cout << objUp << " " << enDown << " " << objLeft << " " << enRight << " " << objDown << " " << enUp << " " << objRight << " " << enLeft << endl;
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


void reGame(Object& pong, vector<Object*>& bars) {
    newRound(pong);
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
    //        Object(ShaderProgram& program, float x = 0, float y = 0, float width = 1, float height = 1, float velocity_x = 0, float velocity_y = 0, bool is = false, GLuint tex = 0);
    
    // initial set up
    
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
    

    Object player(prog, 5, 0, 0.2, 2);
//    player.modelMatrix.Translate(player.x, player.y, 0);
    
    bool enemyUp = true;
    Object enemy(prog, -5, 0, 0.2, 2, 0, 3, false);
    
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
        srand(time(NULL));
        
        // keyboard event
        while (SDL_PollEvent(&event)) checkKeyboard(event, done, restart, regame, player);
        
        if (restart){
            newRound(pong);
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
