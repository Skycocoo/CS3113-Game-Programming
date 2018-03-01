// Yuxi Luo (yl4217), February 26, 2018
// Homework 3, Space Invaders, CS3113 Game Programming

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <cstdlib>
#include <vector>

// define Object as an entity in this game
// header files for shaderprogram & matrix & sdl are included
#include "Util/Object.h"
#include "Util/XMLLoad.h"
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
    collisionDetection(pong, bars);
    
    pong.x += elapsed * pong.velocity_x;
    pong.y += elapsed * pong.velocity_y;
    
    pong.update();
    
    if (pong.x - pong.width / 2 < -screenWidth) {
        playerScore += 1;
        initiatePong(pong);
    } else if (pong.x + pong.width / 2 > screenWidth) {
        enemyScore += 1;
        initiatePong(pong);
    }
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

class SheetSprite {
public:
    SheetSprite(){};
    SheetSprite(unsigned int textureID, float u, float v, float width, float height, float size): textureID(textureID), u(u), v(v), width(width), height(height), size(size){};
    
    void Draw(ShaderProgram *program){
        glBindTexture(GL_TEXTURE_2D, textureID);
        GLfloat texCoords[] = {
            u, v+height,
            u+width, v,
            u, v,
            u+width, v,
            u, v+height,
            u+width, v+height
        };
        float aspect = width / height;
        float vertices[] = {
            -0.5f * size * aspect, -0.5f * size,
            0.5f * size * aspect, 0.5f * size,
            -0.5f * size * aspect, 0.5f * size,
            0.5f * size * aspect, 0.5f * size,
            -0.5f * size * aspect, -0.5f * size ,
            0.5f * size * aspect, -0.5f * size};
        // draw our arrays
    }
    float size;
    unsigned int textureID;
    float u;
    float v;
    float width;
    float height;
};

void Render(SheetSprite& enemySprite, ShaderProgram* program) {
    enemySprite.Draw(program);
}



int main(){
    // initial set up
    srand(time(NULL));
    SDL_Window* displayWindow = setUp("Homework 3");
    
    XMLLoad xml ("Asset/sheet.xml");
    cout << xml.getData("wingYellow_7.png");
    
//    // setting up texts
//    GLuint texture;
//    ShaderProgram shad = setTextured("Asset/sheet.png", texture);
//    SheetSprite t;
//    t.textureID = texture;
//
//
//    SDL_Event event;
//    bool done = false;
//    while (!done) {
//        // keyboard event
//        while (SDL_PollEvent(&event)) checkKeyboard(event, done);
//
//        // display
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        Render(t, &shad);
//
//        SDL_GL_SwapWindow(displayWindow);
//    }
//
//    SDL_Quit();
    
    return 0;
}
