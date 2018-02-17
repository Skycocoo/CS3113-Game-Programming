// Yuxi Luo (yl4217), February 15, 2018
// Homework 2, PONG!, CS3113 Game Programming

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



#include <cstdlib>
#include <iostream>
#include <vector>

// define Object as an entity in this game
// header files for shaderprogram & matrix are included
#include "Object.h"

using namespace std;
float screenRatio;
float screenHeight;
float screenWidth;
float splitScale;


// from lecture slide Jan 31, 2018
GLuint LoadTexture(const char *filePath) {
    int w, h, comp;
    unsigned char* image = stbi_load(filePath, &w, &h, &comp, STBI_rgb_alpha);
    
    if(image == NULL) {
        cout << "Unable to load image in the path " << *filePath << ". Make sure the path is correct\n";
        assert(false);
    }
    
    GLuint retTexture;
    glGenTextures(1, &retTexture);
    glBindTexture(GL_TEXTURE_2D, retTexture);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    stbi_image_free(image);
    return retTexture;
}

// untextured shader
ShaderProgram setUntextured(){
    ShaderProgram program;
    program.Load(RESOURCE_FOLDER"Shaders/vertex.glsl", RESOURCE_FOLDER"Shaders/fragment.glsl");
    glUseProgram(program.programID);
    
    return program;
}

// textured shader
ShaderProgram setTextured(const string& filepath, GLuint& texture){
    ShaderProgram program;
    program.Load(RESOURCE_FOLDER"Shaders/vertex_textured.glsl", RESOURCE_FOLDER"Shaders/fragment_textured.glsl");
    texture = LoadTexture((RESOURCE_FOLDER + filepath).c_str());
    
    return program;
}


// initialize the window
SDL_Window* setUp(){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* displayWindow = SDL_CreateWindow("Homework 2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1020, 720, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
    glViewport(0, 0, 1020, 720);
    glClearColor(0, 0, 0, 0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // screen view point
    screenRatio = float(1020) / float(720);
    screenHeight = 5.0;
    screenWidth = screenHeight * screenRatio;
    
    return displayWindow;
}

// check keyboard event
void checkKeyboard(const SDL_Event& event, bool& done, bool& playerup){
    switch (event.type){
        case SDL_QUIT:
            done = true;
            break;
        case SDL_WINDOWEVENT_CLOSE:
            done = true;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode){
                case SDL_SCANCODE_Q: // quit
                    done = true;
                    break;
                case SDL_SCANCODE_UP: // player control
                    playerup = true;
                    break;
                case SDL_SCANCODE_DOWN: // player control
                    playerup = false;
                    break;
                default:
                    break;
            }
            break;
    }
}


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
//    float distance = elapsed * obj.velocity_y;
//
//    if (obj.y > screenHeight - distance - obj.width / 2 - splitScale / 2) up = false;
//    else if (obj.y < -screenHeight + distance + obj.width / 2 + splitScale / 2) up = true;
//    if (up) obj.y += distance;
//    else obj.y -= distance;
//
    obj.modelMatrix.Identity();
    obj.modelMatrix.Translate(obj.x, obj.y, 0);
}


void initiatePong(Object& obj){
    obj.velocity_y = 0;
    obj.velocity_x = 3;
//    obj.velocity_x = rand() % 10;

    // direction of Pong based on the current scorer?
    
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
        float opUp = bars[i]->y + bars[i]->height / 2,
            opDown = bars[i]->y - bars[i]->height / 2,
            opLeft = bars[i]->x - bars[i]->width / 2,
            opRight = bars[i]->x + bars[i]->width / 2;
        
        // intersecting
        if (objUp > opDown && objLeft < opRight && objDown < opUp && objRight > opLeft){
            collide = true;
        }
    }
    
    if (collide) obj.velocity_x = -obj.velocity_x;
    
    // upper / lower boundary collision
    if ((objUp > screenHeight - splitScale / 2) || (objDown < -screenHeight + splitScale / 2)) {
        obj.velocity_y = -obj.velocity_y;
        cout << obj.velocity_y << endl;
    }
    
}

void updatePong(Object& obj, const vector<Object*>& bars, float elapsed){
    collisionDetection(obj, bars);
    obj.x += elapsed * obj.velocity_x;
    obj.y += elapsed * obj.velocity_y;
    // cos / sin?
    
    obj.modelMatrix.Identity();
    obj.modelMatrix.Translate(obj.x, obj.y, 0);

//    edge detection? -> if x go beyond width / -width: score ++ for player / enemy
    
}

void win(){
//    determine the end of game
//    if one side scores >= 10 then win (display the result & end the game)
//    if not: continue the game
    
}

void dispalyGame(){
//    display the player & computer's position (before the start of the game)
//    display the score for each side in the middle of the game
//    display the sign of an object which Pong will point towards (player or enemy)
    
}

int main(){
    // initial set up
    srand(time(NULL));
    SDL_Window* displayWindow = setUp();
    
    // setting up objects
    ShaderProgram prog = setUntextured();
    
    // split line
    Object split(prog, false);
    vector<Matrix> splitPos;
    splitInit(split, splitPos, 20);
    
    
    // player & enemy (computer control)
    vector<Object*> bars;
    
    bool playerUp = true;
    Object player(prog, false);
    player.x = 5;
    player.velocity_y = 3;
    
    bool enemyUp = true;
    Object enemy(prog, false);
    enemy.x = -5;
    enemy.velocity_y = 3;
    
    bars.push_back(&player);
    bars.push_back(&enemy);
    
    // ping pong
    Object pong(prog, false);
    initiatePong(pong);
    
    cout << pong.velocity_y << pong.velocity_x << endl;

    // game loop
    float lastFrameTicks = 0.0f;
    SDL_Event event;
    bool done = false;
    while (!done) {
        
        // keyboard event
        while (SDL_PollEvent(&event)) checkKeyboard(event, done, playerUp);
        
        // update parameters
        float ticks = (float)SDL_GetTicks()/1000.0f;
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        
        updateSlide(enemy, elapsed, enemyUp);
        updateSlide(player, elapsed, playerUp);
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
