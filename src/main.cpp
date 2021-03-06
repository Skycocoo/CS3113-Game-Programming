// Yuxi Luo (yl4217), Yiyang Zeng
// Final Project, Platformer, CS3113 Game Programming

// !!!Important!!!
// please clone with git clone --recursive https://github.com/Skycocoo/CS3113-Game-Programming.git
// since I'm using gitmodules to include glm

#define RESOURCE_FOLDER "../src/"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <cstdlib>
#include <vector>

#include <SDL_mixer.h>
#include "includes.hpp"

using namespace std;

///////////////////////////////////////////GLOBAL VARIABLES///////////////////////////////////////////

float screenRatio = 0.0, screenHeight = 0.0, screenWidth = 0.0, splitScale = 0.0, edge = 1.0;
ShaderProgram textured, untextured, lighting;

float fixedStep = 0.0166666f; // 60 FPS (1.0f/60.0f) (update sixty times a second)
int maxStep = 3;

enum GameMode {STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER};
GameMode mode = STATE_MAIN_MENU;

Mix_Chunk* jump;
Mix_Chunk* walk;

glm::vec3 startPos = glm::vec3(0, 0, 0);

//////////////////////////////////////////////////////////////////////////////////////////////////////



void updateGame(const SDL_Event& event, GameState& game){
    // switch (event.type){
        // case SDL_KEYDOWN:
    if (event.type == SDL_KEYDOWN){
        switch (event.key.keysym.scancode){
            case SDL_SCANCODE_B:
                if (mode == STATE_MAIN_MENU) mode = STATE_GAME_LEVEL;
                game.init();
                break;
            case SDL_SCANCODE_LEFT:
                if (mode == STATE_GAME_LEVEL) {
                    Mix_PlayChannel(1, walk, 0);
                    game.player1.setVelo(-0.3);
                }
                break;
            case SDL_SCANCODE_RIGHT:
                if (mode == STATE_GAME_LEVEL) {
                    Mix_PlayChannel(1, walk, 0);
                    game.player1.setVelo(0.3);
                }
                break;
            case SDL_SCANCODE_UP:
                if (mode == STATE_GAME_LEVEL) {
                    Mix_PlayChannel(-1, jump, 0);
                    if (game.player1.numJump < 3) {
                        if (!game.player1.superJump) game.player1.jump(1.2);
                        else {
                            game.player1.jump(4);
                            game.player1.superJump = false;
                        }
                        game.player1.numJump += 1;
                    }
                }
                break;

            case SDL_SCANCODE_A:
                if (mode == STATE_GAME_LEVEL) {
                    Mix_PlayChannel(1, walk, 0);
                    game.player2.setVelo(-0.3);
                }
                break;
            case SDL_SCANCODE_D:
                if (mode == STATE_GAME_LEVEL) {
                    Mix_PlayChannel(1, walk, 0);
                    game.player2.setVelo(0.3);
                }
                break;
            case SDL_SCANCODE_W:
                if (mode == STATE_GAME_LEVEL) {
                    Mix_PlayChannel(-1, jump, 0);
                    if (game.player2.numJump < 3) {
                        if (!game.player2.superJump) game.player2.jump(1.2);
                        else {
                            game.player2.jump(4);
                            game.player2.superJump = false;
                        }
                        game.player2.numJump += 1;
                    }
                }
                break;
            case SDL_SCANCODE_P:
                if (mode == STATE_GAME_LEVEL){
                    game.cheat();
                }
                break;
        }
    }
}



int main(){
    // initial set up
    SDL_Window* displayWindow = setUp("Finanl project");

    // music
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    Mix_Music* music = Mix_LoadMUS(RESOURCE_FOLDER"Asset/my_music.mp3");
    jump = Mix_LoadWAV(RESOURCE_FOLDER"Asset/jump.wav");
    walk = Mix_LoadWAV(RESOURCE_FOLDER"Asset/walk.wav");
    if(!music || !jump || !walk) {
       cout << "Mix_Load: ";
       Mix_GetError();
       cout << endl;
       exit(1);
    }
    Mix_PlayMusic(music, -1);


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
