// Yuxi Luo (yl4217), February 26, 2018
// Homework 3, Space Invaders, CS3113 Game Programming

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <cstdlib>
#include <vector>

#include "Util/Util.h"
#include "setUp.h"

using namespace std;
float screenRatio;
float screenHeight;
float screenWidth;
float splitScale;


// display the score for each side in the middle of the game
void displayGame(Text& disp){
    disp.display("a", 0.8, 1, 0, 0);
}


int main(){
    // initial set up
    srand(time(NULL));
    SDL_Window* displayWindow = setUp("Homework 3 Space Invaders");
    
    
    XMLLoad xml ("Asset/sheet.xml");

    GLuint texture;
    ShaderProgram shad = setTextured("Asset/sheet.png", texture);
    Object sprite(shad, texture);
    sprite.setupData(xml.getData("playerShip2_orange.png"));
    
    GLuint texture2;
    ShaderProgram shad2 = setTextured("Asset/font1.png", texture2);
    Text disp(shad2, texture2);

    SDL_Event event;
    bool done = false;
    while (!done) {
        // keyboard event
        while (SDL_PollEvent(&event)) checkKeyboard(event, done);

        // display
        glClear(GL_COLOR_BUFFER_BIT);

        sprite.display();
        displayGame(disp);
        
        SDL_GL_SwapWindow(displayWindow);
    }

    SDL_Quit();
    
    return 0;
}
