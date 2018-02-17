// Yuxi Luo (yl4217), February 15, 2018
// Homework 2, PONG!, CS3113 Game Programming

#include <iostream>
#include <vector>

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "Matrix.h"

#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// define Object as an entity in this game
#include "Object.h"

using namespace std;
float screenRatio;
float screenHeight;
float screenWidth;


// from lecture slide Jan 31, 2018
GLuint LoadTexture(const char *filePath) {
    int w, h, comp;
    unsigned char* image = stbi_load(filePath, &w, &h, &comp, STBI_rgb_alpha);
    
    if(image == NULL) {
        std::cout << "Unable to load image in the path " << *filePath << ". Make sure the path is correct\n";
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
                case SDL_SCANCODE_Q:
                    done = true;
                    break;
                case SDL_SCANCODE_UP:
                    playerup = true;
                    break;
                case SDL_SCANCODE_DOWN:
                    playerup = false;
                    break;
                default:
                    break;
            }
            break;
    }
}


// calculate the position of split line in advance;
// return the size of tiles for collision detection
float splitInit(const Object& obj, vector<Matrix>& splitPos, int num){
    float splitScale = float(2) / float(num);
    for(int i = 0; i < num; i++){
        float relative = (i - float(num-1) / float(2)) * 2 * screenHeight / float(num);
        
        Matrix pos;
        pos.Translate(0, relative, 0);
        pos.Scale(splitScale, splitScale, splitScale);
        
        splitPos.push_back(pos);
    }

    
    // draw upper & lower boundaries
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
    
    return splitScale;
}


// draw the middle line to split the window into two parts
void drawSplit(Object& obj, const vector<Matrix>& splitPos){
    for(int i = 0; i < splitPos.size(); i++){
        obj.modelMatrix = splitPos[i];
        obj.display();
    }
}

// update the slide bar to be automatically moved by time
void updateSlide(Object& obj, float elapsed, bool& up, float splitscale){
    float distance = elapsed * obj.velocity_y;
    
    if (obj.y > screenHeight - distance - obj.width / 2 - splitscale / 2) up = false;
    else if (obj.y < -screenHeight + distance + obj.width / 2 + splitscale / 2) up = true;
    if (up) obj.y += distance;
    else obj.y -= distance;
    
    obj.modelMatrix.Identity();
    obj.modelMatrix.Translate(obj.x, obj.y, 0);
}


void initiatePong(Object& obj){
//    random vector for velocity?

//    direction of Pong based on the current scorer?
    
}

void updatePong(Object& obj, float elapsed){
//    if collide: with upper/lower boundary : velocity y = - velocity y
//        with bars: verlocity x = - velocity x
//
    
//    collision detection? -> change the direction of velocity
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
    SDL_Window* displayWindow = setUp();
    
    // setting up objects
    ShaderProgram prog = setUntextured();
    
    // split line
    Object split(prog, false);
    vector<Matrix> splitPos;
    float splitScale = splitInit(split, splitPos, 20);
    
    
    // player & enemy (computer control)
    bool playerUp = true;
    Object player(prog, false);
    player.x = 5;
    player.velocity_y = 3;
    
    bool enemyUp = true;
    Object enemy(prog, false);
    enemy.x = -5;
    enemy.velocity_y = 3;
    
    
    // ping pong
    Object ping(prog, false);
    

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
        
        updateSlide(enemy, elapsed, enemyUp, splitScale);
        updateSlide(player, elapsed, playerUp, splitScale);
        
        // display
        glClear(GL_COLOR_BUFFER_BIT);
        
        drawSplit(split, splitPos);
        enemy.display();
        player.display();
        
        SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}
