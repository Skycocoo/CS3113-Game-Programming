// Yuxi Luo (yl4217), February 26, 2018
// Homework 4, Platformers, CS3113 Game Programming

// !!!Important!!!
// please clone with git clone --recursive https://github.com/Skycocoo/CS3113-Game-Programming.git
// since I'm using gitmodules to include glm


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <cstdlib>
#include <vector>

#include "Util/Util.h"
#include "setUp.h"

using namespace std;

///////////////////////////////////////////GLOBAL VARIABLES///////////////////////////////////////////

float screenRatio = 0.0, screenHeight = 0.0, screenWidth = 0.0, splitScale = 0.0, edge = 1.0;
ShaderProgram textured, untextured;

float fixedStep = 0.0166666f; // 60 FPS (1.0f/60.0f) (update sixty times a second)
int maxStep = 3;

enum GameMode {STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER};
GameMode mode = STATE_GAME_LEVEL;

//////////////////////////////////////////////////////////////////////////////////////////////////////



void updateGame(const SDL_Event& event, GameState& game){
    switch (event.type){
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode){
                case SDL_SCANCODE_B:
                    if (mode == STATE_GAME_OVER) game.init();
                    mode = STATE_GAME_LEVEL;
                    break;
                case SDL_SCANCODE_LEFT:
                    if (mode == STATE_GAME_LEVEL) game.player.control(-5);
                    break;
                case SDL_SCANCODE_RIGHT:
                    if (mode == STATE_GAME_LEVEL) game.player.control(5);
                    break;
            }
            break;
    }
}



int main(){
    // initial set up
    srand(time(NULL));
    SDL_Window* displayWindow = setUp("Homework 3 Space Invaders");

    GameState game;

    SDL_Event event;
    bool done = false;
    float lastFrameTicks = 0.0f, accumulator = 0.0f;

    while (!done) {
        // keyboard event
        while (SDL_PollEvent(&event)) {
            checkKeyboard(event, done);
            updateGame(event, game);
        }

        game.fixedUpdate(lastFrameTicks, accumulator);

        // display
        glClear(GL_COLOR_BUFFER_BIT);
        game.render();
        SDL_GL_SwapWindow(displayWindow);
    }

    SDL_Quit();

    return 0;
}
