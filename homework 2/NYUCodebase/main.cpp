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

using namespace std;
float screenRatio;
float screenHeight;
float screenWidth;

// create an object class to handle parameters
class Object{
public:
    // matrices
    Matrix projectionMatrix;
    Matrix modelMatrix;
    Matrix viewMatrix;
    
    // positions
    float x = 0;
    float y = 0;
    float width = 1;
    float height = 1;
    float velocity_x = 1;
    float velocity_y = 1;
    
    Object(ShaderProgram& program, bool is = false, GLuint tex = 0):
    program(&program), istexture(is), texture(tex){
        projectionMatrix.SetOrthoProjection(-screenWidth, screenWidth, -screenHeight, screenHeight, -1.0f, 1.0f);
    }

    void display(){
        program->SetModelMatrix(modelMatrix);
        program->SetProjectionMatrix(projectionMatrix);
        program->SetViewMatrix(viewMatrix);
        
        glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
        glEnableVertexAttribArray(program->positionAttribute);
        
        if (istexture){
            glBindTexture(GL_TEXTURE_2D, texture);
            glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
            glEnableVertexAttribArray(program->texCoordAttribute);
        }
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(program->positionAttribute);
        
        if (istexture) glDisableVertexAttribArray(program->texCoordAttribute);
        
    }
    
private:
    ShaderProgram* program;
    
    bool istexture = false;
    GLuint texture;
    
    vector<float> vertices = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
    vector<float> texCoords = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};

    
//    float rotation;


};


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
    program.Load(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
    glUseProgram(program.programID);
    
    return program;
}

// textured shader
ShaderProgram setTextured(const string& filepath, GLuint& texture){
    ShaderProgram program;
    program.Load(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
    texture = LoadTexture((RESOURCE_FOLDER + filepath).c_str());
    
    return program;
}

// check keyboard event
void checkKeyboard(const SDL_Event& event, bool& done){
    switch (event.type){
        case SDL_QUIT:
            done = true;
            break;
        case SDL_WINDOWEVENT_CLOSE:
            done = true;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.scancode == SDL_SCANCODE_Q) done = true;
            break;
    }
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

// calculate the position of split line in advanceg
void splitInit(vector<Matrix>& splitPos, int num){
    float splitScale = float(2) / float(num);
    
    for(int i = 0; i < num; i++){
        float relative = (i - float(num-1) / float(2)) * 2 * screenHeight / float(num);
        
        Matrix pos;
        pos.Translate(0, relative, 0);
        pos.Scale(splitScale, splitScale, splitScale);
        
        splitPos.push_back(pos);
    }
}

// draw the middle line to split the window into two parts
void drawSplit(Object& obj, const vector<Matrix>& splitPos){
    
    for(int i = 0; i < splitPos.size(); i++){
        obj.modelMatrix = splitPos[i];
        obj.display();
    }
}

void updateEnemy(Object& enemy, float elapsed, bool& up){
//    bool up = true;
    float distance = elapsed * enemy.velocity_y;
    
    if (enemy.y > screenHeight - distance - enemy.width / 2) up = false;
    else if (enemy.y < -screenHeight + distance + enemy.width / 2) up = true;
    
    if (up) enemy.y += distance;
    else enemy.y -= distance;
    
    enemy.modelMatrix.Identity();
    enemy.modelMatrix.Translate(enemy.x, enemy.y, 0);
}

int main(){
    // initial set up
    SDL_Window* displayWindow = setUp();
    
    // setting up objects
    ShaderProgram prog = setUntextured();
    
    // split line
    Object split(prog, false);
    vector<Matrix> splitPos;
    splitInit(splitPos, 20);
    
    
    // player & enemy (computer control)
    Object player(prog, false);
    player.x = 5;
    
    
    bool upward = true;
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
        while (SDL_PollEvent(&event)) checkKeyboard(event, done);
        
        // update parameters
        float ticks = (float)SDL_GetTicks()/1000.0f;
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        
        updateEnemy(enemy, elapsed, upward);
        
        
        // display
        glClear(GL_COLOR_BUFFER_BIT);
        drawSplit(split, splitPos);
        
        enemy.display();
        
        SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}
