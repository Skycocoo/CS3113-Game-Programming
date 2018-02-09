// Templated by Prof Safrin
// Modified by Yuxi Luo, February 9, 2018
// Homework 1, CS3113 Game Programming

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include <iostream>
#include "ShaderProgram.h"
#include "Matrix.h"

using namespace std;

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

ShaderProgram setup(){
    glViewport(0, 0, 640, 360);
    
    ShaderProgram program;
    program.Load(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
    glUseProgram(program.programID);
    
    return program;
}

void display(ShaderProgram& program, const Matrix& modelMatrix, const Matrix& projectionMatrix, const Matrix&(viewMatrix)){
    
    program.SetModelMatrix(modelMatrix);
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    float vertices[] = {0.5f, -0.5f, 0.0f, 0.5f, -0.5f, -0.5f};
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(program.positionAttribute);
}

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

int main(int argc, char *argv[]){
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Homework 1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
    #ifdef _WINDOWS
    glewInit();
    #endif
    
    ShaderProgram prog = setup();
    
    // matrices
    Matrix projectionMatrix;
    Matrix modelMatrix;
    Matrix viewMatrix;
    projectionMatrix.SetOrthoProjection(-3.55, 3.55, -2.0f, 2.0f, -1.0f, 1.0f);
    
    SDL_Event event;
    bool done = false;
    // game loop
    while (!done) {
        // check keyboard event
        while (SDL_PollEvent(&event)) checkKeyboard(event, done);
        
        // display contents
        glClear(GL_COLOR_BUFFER_BIT);
        display(prog, modelMatrix, projectionMatrix, viewMatrix);
        SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}
