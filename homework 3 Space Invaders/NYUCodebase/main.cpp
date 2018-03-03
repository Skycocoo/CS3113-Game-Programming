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
    Bullet(ShaderProgram* program, glm::vec3 pos, glm::vec3 velo = glm::vec3(0, 0.7, 0)): Object(program, 0, pos, velo){
        Object::setShape(glm::vec3(0.05, 0.3, 0));
    }
    
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
        this->display();
        for(size_t i = 0; i < bul.size(); i++) bul[i].display();
    }
    
    // max amount of bullets: 20
    void addBullet(){
        // bullet from current position
        if (bul.size() < 20) bul.push_back(Bullet(program, pos));
    }
    
    // remove the bullet when collide
    void delBullet(int index){
        // bul.erase(bul.begin() + index);
        bul[index] = bul[bul.size() - 1];
        bul.pop_back();
    }
    
    int getLives() const { return lives; }
    void decLives(){ lives -= 1; }

private:
    int lives = 5;
};


// dont have to inherit from object
class EnemyGroup{
private: class Enemy;
public:
    vector<Enemy> ene;
    
    // enemy group don't use shaderprogram itself
    // shaderprogram & texture & data is for enemy
    // need to restructure enemygroup
    EnemyGroup(ShaderProgram* program, GLuint texture, const XMLData& data, glm::vec3 pos, glm::vec3 velo = glm::vec3(0.1, 0, 0), int numEn = 15, int numCol = 5): numEn(numEn), numCol(numCol), numRow(numEn/numCol), pos(pos){
        
        // create enemy objects
        float posX = pos.x, posY = pos.y, size = 0.5, spacing = 0.5;
        float step = size + spacing;
        
        for (int i = 0; i < numRow; i++){
            float relativeY = i - float(numRow - 1) / float(2);
            
            for (int j = 0; j < numCol; j++){
                float relativeX = j - float(numCol - 1) / float(2);
                Enemy temp (program, texture, data, glm::vec3(posX + relativeX * step, posY + relativeY * step, 0));
                temp.setScale(size);
                ene.push_back(temp);
            }
        }
        
        // need to update the shape of enemygroup
        shape = glm::vec3(4 * step + size, 2 * step + size, 0);
    }
    
    void update(float elapsed){
        // loop throug every enemy
        // if approach to the edge : reverse sign of velocity x for every enemy
        // control addbullet()
        for (size_t i = 0; i < ene.size(); i++) ene[i].update(elapsed);
    }
    
    void display(){
        // loop throug every enemy
        for (size_t i = 0; i < ene.size(); i++) ene[i].display();
    }

private:
    int numEn;
    int numCol;
    int numRow;
    
    glm::vec3 pos;
    glm::vec3 shape;
    
    class Enemy: public Object{
    public:
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
    ShaderProgram tex1 = setTextured("Asset/sheet.png", texture);
    Object sprite(&tex1, texture);
    sprite.setData(xml.getData("playerShip2_orange.png"));

    GLuint texture2;
    ShaderProgram tex2 = setTextured("Asset/font1.png", texture2);
    Text disp(&tex2, texture2);

    ShaderProgram untex = setUntextured();
    Bullet bul(&untex, glm::vec3(0, -3, 0));
    
    EnemyGroup ene(&tex1, texture, xml.getData("playerShip2_orange.png"), glm::vec3(0));

    SDL_Event event;
    bool done = false;
    float lastFrameTicks = 0.0f;
    while (!done) {
        // keyboard event
        while (SDL_PollEvent(&event)) checkKeyboard(event, done);
        
        // update parameters
        float ticks = (float)SDL_GetTicks()/1000.0f;
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        
        bul.update(elapsed);
        ene.update(elapsed);
        
        // display
        glClear(GL_COLOR_BUFFER_BIT);
        
        sprite.display();
        ene.display();

        SDL_GL_SwapWindow(displayWindow);
    }

    SDL_Quit();

    return 0;
}
