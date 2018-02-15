// Templated by Prof Safrin
// Modified by Yuxi Luo (yl4217), February 9, 2018
// Homework 1, CS3113 Game Programming


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

// create an object class to handle parameters
class Object{
    
public:
    Object(const ShaderProgram& program, bool is, GLuint tex = 0): program(program), istexture(is), texture(tex){
        projectionMatrix.SetOrthoProjection(-5.0f, 5.0f, -5.0f, 5.0f, -1.0f, 1.0f);
    }
    
    void animate(float ticks, size_t i = 0){
        modelMatrix = Matrix();
        modelMatrix.Scale(0.5f, 0.5f, 0.5f);
        modelMatrix.Translate(float(6) / float(i) * cos(ticks * i), float(6) / float(i) * sin(ticks * i), 0);
    }
    
    void display(){
        program.SetModelMatrix(modelMatrix);
        program.SetProjectionMatrix(projectionMatrix);
        program.SetViewMatrix(viewMatrix);
        
        if (!istexture){
            float vertices[] = {0.5f, -0.5f, 0.0f, 0.5f, -0.5f, -0.5f};
            glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
            glEnableVertexAttribArray(program.positionAttribute);
            
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glDisableVertexAttribArray(program.positionAttribute);
        } else {
            glBindTexture(GL_TEXTURE_2D, texture);
            
            float vertices[] = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
            glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
            glEnableVertexAttribArray(program.positionAttribute);
            
            float texCoords[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
            glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
            glEnableVertexAttribArray(program.texCoordAttribute);
            
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glDisableVertexAttribArray(program.positionAttribute);
            glDisableVertexAttribArray(program.texCoordAttribute);
        }
    }
    
private:
    ShaderProgram program;
    
    GLuint texture;
    Matrix projectionMatrix;
    Matrix modelMatrix;
    Matrix viewMatrix;
    
    bool istexture = false;
    
    float x;
    float y;
    float rotation;
    int textureID;
    float width;
    float height;
    float velocity_x;
    float velocity_y;
    
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

SDL_Window* setUp(){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* displayWindow = SDL_CreateWindow("Homework 2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 640, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
    glViewport(0, 0, 640, 640);
    glClearColor(1, 1, 1, 1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    return displayWindow;
}

int main(){
    // initial set up
    SDL_Window* displayWindow = setUp();
    
    // loading objects
    vector<Object> objects;
    
    ShaderProgram prog = setUntextured();
    objects.push_back(Object(prog, false));
    
    GLuint texture1;
    ShaderProgram prog1 = setTextured("starBronze.png", texture1);
    objects.push_back(Object(prog1, true, texture1));
    
    GLuint texture2;
    ShaderProgram prog2 = setTextured("starSilver.png", texture2);
    objects.push_back(Object(prog2, true, texture2));
    
    GLuint texture3;
    ShaderProgram prog3 = setTextured("starGold.png", texture3);
    objects.push_back(Object(prog3, true, texture3));
    
    
    // game loop
    SDL_Event event;
    bool done = false;
    while (!done) {
        
        // check keyboard event; could type q to escape
        while (SDL_PollEvent(&event)) checkKeyboard(event, done);
        
        // display contents
        glClear(GL_COLOR_BUFFER_BIT);
        
        float ticks = (float) SDL_GetTicks()/600.0f;
        
        // animate the objects
        for (size_t i = 1; i < objects.size(); i++) objects[i].animate(ticks, i);
        
        // display the objects
        for (Object& obj: objects) obj.display();
        
        SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}
