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
GameMode mode = STATE_MAIN_MENU;

//////////////////////////////////////////////////////////////////////////////////////////////////////


// to do: seperate enemygroup from GameState
class Pong: public GameState{
public:
    Pong(): GameState::GameState(){
        init();
    }
    
    void init(){
        
    }
};



class SpaceInvader: public GameState{
public:
    SpaceInvader(): GameState::GameState(){
        init();
    }
    
    void init(){
        
        xml = XMLLoad("Asset/sheet.xml");
        
        
        GLuint texture;
        textured = setTextured("Asset/sheet.png", texture);
        
        std::vector<XMLData> playerlife;
        playerlife.push_back(xml.getData("playerShip1_blue.png"));
        playerlife.push_back(xml.getData("playerShip1_damage1.png"));
        playerlife.push_back(xml.getData("playerShip1_damage2.png"));
        playerlife.push_back(xml.getData("playerShip1_damage3.png"));
        playerlife.push_back(xml.getData("playerLife1_blue.png"));
        
        enemygroup = EnemyGroup(texture, xml.getData("enemyBlack1.png"), glm::vec3(0, 2, 0));
        player = Player(texture, playerlife, glm::vec3(0, -4, 0));
    }

    
    
    void displayMainMenu(){
        disp.render("Space Invaders", 1, 2, 0, 3.5);
        disp.render("<=   =>   to move", 0.5, 1, 0, 1);
        disp.render("[     ]  to fight", 0.5, 1, 0, 0);
        
        disp.render("B: begin   Q: quit", 0.5, 1, 0, -1.5);
    }
    
    void displayLevel(){
        player.render();
        enemygroup.render();
        
        disp.render("Score: " + std::to_string(player.getScore()), 0.4, 1, -4, 3.5);
        disp.render("Lives: ", 0.4, 1, 3.5, 3.5);
        player.renderLives();
    }
    
    void displayOver(){
        disp.render("Game Over", 1, 2, 0, 1.5);
        
        std::string winner = (player.getLives() == 0) ? "Enemy" : "Player";
        disp.render(winner + " wins", 1, 2, 0, 0);
        
        disp.render("B: begin   Q: quit", 0.5, 1, 0, -1.5);
    }
};




void updateGame(const SDL_Event& event, GameState& game){
    switch (event.type){
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode){
                case SDL_SCANCODE_B:
                    if (mode == STATE_GAME_OVER) game.init();
                    mode = STATE_GAME_LEVEL;
                    break;
                case SDL_SCANCODE_SPACE:
                    if (mode == STATE_GAME_LEVEL) game.player.addBullet();
                    break;
                case SDL_SCANCODE_LEFT:
                    if (mode == STATE_GAME_LEVEL) game.player.control(-1);
                    break;
                case SDL_SCANCODE_RIGHT:
                    if (mode == STATE_GAME_LEVEL) game.player.control(1);
                    break;
            }
            break;
    }
}



int main(){
    // initial set up
    srand(time(NULL));
    SDL_Window* displayWindow = setUp("Homework 3 Space Invaders");

    SpaceInvader game;
    
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
