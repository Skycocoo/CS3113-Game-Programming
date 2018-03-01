// Yuxi Luo (yl4217), February 26, 2018
// Homework 3, Space Invaders, CS3113 Game Programming

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <cstdlib>
#include <vector>

#include "Util/Text.h"
#include "Util/Object.h"
#include "Util/XMLLoad.h"
#include "setUp.h"

using namespace std;
float screenRatio;
float screenHeight;
float screenWidth;
float splitScale;


// display the score for each side in the middle of the game
void displayGame(Text& disp){
    disp.display("test", 0.8, 1, 0, 0);
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
    
//    XMLLoad xml ("Asset/sheet.xml");
//    cout << xml.getData("wingYellow_7.png");
    
    // setting up texts
    GLuint texture;
    ShaderProgram shad = setTextured("Asset/sheet.png", texture);
    SheetSprite t;
    t.textureID = texture;

    GLuint texture2;
    ShaderProgram shad2 = setTextured("Asset/font1.png", texture2);
    Text disp(shad2, true, texture2);

    SDL_Event event;
    bool done = false;
    while (!done) {
        // keyboard event
        while (SDL_PollEvent(&event)) checkKeyboard(event, done);

        // display
        glClear(GL_COLOR_BUFFER_BIT);

//        Render(t, &shad);
        displayGame(disp);
        
        SDL_GL_SwapWindow(displayWindow);
    }

    SDL_Quit();
    
    return 0;
}
