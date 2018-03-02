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

enum GameMode{STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER};


class Bullet: public Object{
public:
    Bullet(ShaderProgram* program, glm::vec3 pos, glm::vec3 velo = glm::vec3(2.0)): Object(program, 0, pos, velo){}
    void update(float elapsed){
        pos += elapsed * velo;
        Object::update();
    }
};


class Player: public Object{
public:
    vector<Bullet> bul;
    
    
    Player(ShaderProgram* program, GLuint texture, const XMLData& data, glm::vec3 pos = glm::vec3(0, -3, 0)): Object(program, texture, pos){
        Object::setData(data);
    }

    void update(float distance){
        pos.x += distance;
        Object::update();
    }
    
    void display(){
        // player display & palyer's bullet display
    }
    
    void addBullet(){
        // if (bul.size() > 20) return;
    }
    void delBullet(int index){
        // remove the bullet when collide
    }
    
    int getLives() const {return lives;}
    void decLives(){lives -= 1;}

private:
    int lives = 5;
};



class EnemyGroup: public Object{
private: class Enemy;
public:
    vector<Enemy> ene;
    
    EnemyGroup(ShaderProgram* program, GLuint texture, const XMLData& data, glm::vec3 pos, glm::vec3 velo = glm::vec3(0.1, 0, 0), int numEn = 15, int numRow = 5): Object(program, 0, pos, velo), numEn(numEn), numRow(numRow){
        // create enemy objects
        
        // need to update the shape of enemygroup (by calculating the numen & numrow)
    }
    
    void update(float elapsed){
        // loop throug every enemy
        // if approach to the edge : reverse sign of velocity x for every enemy
        // control addbullet()
    }
    
    void display(){
        // loop throug every enemy
    }

private:
    int numEn;
    int numRow;
    
    class Enemy: public Object{
        vector<Bullet> bul;
        
        Enemy(ShaderProgram* program, GLuint texture, const XMLData& data, glm::vec3 pos, glm::vec3 velo = glm::vec3(0.1, 0, 0)): Object(program, texture, pos, velo){
            Object::setData(data);
        }
        
        void addBullet(){
            // if (bul.size() > 20) return;
        }
        
        void delBullet(int index){
            // remove the bullet when collide
        }
        
        void update(float elapsed){
            pos += elapsed * velo;
            Object::update();
        }
    };

};


class GameState{
public:
    GameState(Object& player, Object& enemygroup): player(&player), enemygroup(&enemygroup){}
    
    void checkCollision(){
        // bullets:
        // players' bullet: collide with enemy / with board
        // enemy's bullet: collide with player / with board
        
        // delbullet when collide
    }
    
    void update(float elapsed){
        // player, enemygroup update
        // collision detection
    }
    
    void display(){
        
    }
    
private:
    Object* player;
    Object* enemygroup;
};






int main(){
    // initial set up
    srand(time(NULL));
    SDL_Window* displayWindow = setUp("Homework 3 Space Invaders");

    XMLLoad xml ("Asset/sheet.xml");

    GLuint texture;
    ShaderProgram shad = setTextured("Asset/sheet.png", texture);
    Object sprite(&shad, texture);
    sprite.setData(xml.getData("playerShip2_orange.png"));

    GLuint texture2;
    ShaderProgram shad2 = setTextured("Asset/font1.png", texture2);
    Text disp(&shad2, texture2);



    SDL_Event event;
    bool done = false;
    while (!done) {
        // keyboard event
        while (SDL_PollEvent(&event)) checkKeyboard(event, done);

        // display
        glClear(GL_COLOR_BUFFER_BIT);

        sprite.display();

        SDL_GL_SwapWindow(displayWindow);
    }

    SDL_Quit();

    return 0;
}
