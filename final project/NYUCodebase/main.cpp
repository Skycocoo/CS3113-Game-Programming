// Yuxi Luo (yl4217), February 26, 2018
// Homework 4, Platformers, CS3113 Game Programming

// !!!Important!!!
// please clone with git clone --recursive https://github.com/Skycocoo/CS3113-Game-Programming.git
// since I'm using gitmodules to include glm


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <cstdlib>
#include <vector>

#include "includes.hpp"

using namespace std;

///////////////////////////////////////////GLOBAL VARIABLES///////////////////////////////////////////

float screenRatio = 0.0, screenHeight = 0.0, screenWidth = 0.0, splitScale = 0.0, edge = 1.0;
ShaderProgram textured, untextured;

float fixedStep = 0.0166666f; // 60 FPS (1.0f/60.0f) (update sixty times a second)
int maxStep = 3;

enum GameMode {STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER};
GameMode mode = STATE_GAME_LEVEL;

Mix_Chunk* jump;
Mix_Chunk* walk;

glm::vec3 center = glm::vec3(0, 0, 0);

//////////////////////////////////////////////////////////////////////////////////////////////////////



void updateGame(const SDL_Event& event, GameState& game){
    // switch (event.type){
        // case SDL_KEYDOWN:
    if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP){
        switch (event.key.keysym.scancode){
            case SDL_SCANCODE_B:
                game.init();
                break;
            case SDL_SCANCODE_LEFT:
                if (mode == STATE_GAME_LEVEL) {
                    Mix_PlayChannel(1, walk, 0);
                    game.player1.setAcce(-3);
                }
                break;
            case SDL_SCANCODE_RIGHT:
                if (mode == STATE_GAME_LEVEL) {
                    Mix_PlayChannel(1, walk, 0);
                    game.player1.setAcce(3);
                }
                break;
            case SDL_SCANCODE_UP:
                if (mode == STATE_GAME_LEVEL) {
                    Mix_PlayChannel(-1, jump, 0);
                    game.player1.jump(0.8);
                }
                break;
            // case SDL_SCANCODE_SPACE:
            //     if (mode == STATE_GAME_LEVEL) {
            //         Mix_PlayChannel(-1, jump, 0);
            //         game.player.jump(0.8);
            //     }
            //     break;

            case SDL_SCANCODE_A:
                if (mode == STATE_GAME_LEVEL) {
                    Mix_PlayChannel(1, walk, 0);
                    game.player2.setAcce(-3);
                }
                break;
            case SDL_SCANCODE_D:
                if (mode == STATE_GAME_LEVEL) {
                    Mix_PlayChannel(1, walk, 0);
                    game.player2.setAcce(3);
                }
                break;
            case SDL_SCANCODE_W:
                if (mode == STATE_GAME_LEVEL) {
                    Mix_PlayChannel(-1, jump, 0);
                    game.player2.jump(0.8);
                }
                break;
        }
        // break;
    }
}



int main(){
    // initial set up
    srand(time(NULL));
    SDL_Window* displayWindow = setUp("Finan project");

    // music
    // Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 );
    // Mix_Music* music = Mix_LoadMUS("Asset/my_music.mp3");
    // jump = Mix_LoadWAV("Asset/jump.wav");
    // walk = Mix_LoadWAV("Asset/walk.wav");
    // if(!music || !jump || !walk) {
    //    cout << "Mix_Load: ";
    //    Mix_GetError();
    //    cout << endl;
    //    exit(1);
    // }
    // Mix_PlayMusic(music, -1);


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
