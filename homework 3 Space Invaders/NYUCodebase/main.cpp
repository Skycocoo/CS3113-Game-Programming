// Yuxi Luo (yl4217), February 26, 2018
// Homework 3, Space Invaders, CS3113 Game Programming

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <cstdlib>
#include <vector>

#include "Util/Util.h"
#include "setUp.h"

using namespace std;


float screenRatio = 0.0, screenHeight = 0.0, screenWidth = 0.0, splitScale = 0.0;
ShaderProgram textured;
//ShaderProgram untextured;

enum GameMode{STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER};



class Bullet: public Object{
public:
    Bullet(GLuint texture, const XMLData& data, const glm::vec3& pos, const glm::vec3& velo = glm::vec3(0, 0.7, 0)): Object(&textured, texture, pos, velo){
        Object::setShape(glm::vec3(0.05, 0.3, 0));
        Object::setData(data);
    }
    
    void update(float elapsed){
        pos += elapsed * velo;
        Object::update();
    }
};


class Player: public Object{
public:
    vector<Bullet> bul;
    
    Player(GLuint texture, const XMLData& data, const XMLData& bullet, glm::vec3 pos = glm::vec3(0, -3, 0)): Object(&textured, texture, pos), bullet(bullet){
        Object::setData(data);
    }

    void update(float distance){
        pos.x += distance;
        Object::update();
    }
    
    void render(){
        this->render();
        for(size_t i = 0; i < bul.size(); i++) bul[i].render();
    }
    
    // max amount of bullets: 20
    void addBullet(){
        // bullet from current position
        if (bul.size() < 20) bul.push_back(Bullet(texture, bullet, pos));
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
    XMLData bullet;
};


class EnemyGroup{
class Enemy;
    
public:
    vector<Enemy> ene;
    
    // enemy group don't use shaderprogram itself
    // shaderprogram & texture & data is for enemy
    // need to restructure enemygroup
    EnemyGroup(GLuint texture, const XMLData& data, const XMLData& bullet, const glm::vec3& pos, const glm::vec3& velo = glm::vec3(2, 0, 0), int numEn = 15, int numCol = 5): numEn(numEn), numCol(numCol), numRow(numEn/numCol), pos(pos), velo(velo), bullet(bullet){
        
        // create enemy objects
        float posX = pos.x, posY = pos.y, size = 1, spacing = 0.5;
        float step = size + spacing;
        
        for (int i = 0; i < numRow; i++){
            float relativeY = i - float(numRow - 1) / float(2);
            
            for (int j = 0; j < numCol; j++){
                float relativeX = j - float(numCol - 1) / float(2);
                Enemy temp (texture, data, glm::vec3(posX + relativeX * step, posY + relativeY * step, 0), velo);
                temp.setScale(size);
                ene.push_back(temp);
            }
        }
        
        // need to update the shape of enemygroup
        shape = glm::vec3(4 * step + size, 2 * step + size, 0);
    }
    
    void update(float elapsed){
        // update position for enemy group
        pos += elapsed * velo;
        
        // update position for enemies
        // if approach to the edge : reverse sign of every velocity x
        if (((pos.x - shape.x / 2 < -screenWidth) && (velo.x < 0))||((pos.x + shape.x / 2 > screenWidth) && (velo.x > 0))){
            velo.x = -velo.x;
            for (size_t i = 0; i < ene.size(); i++){
                ene[i].setVelo(velo);
                ene[i].update(elapsed);
            }
        } else for (size_t i = 0; i < ene.size(); i++) ene[i].update(elapsed);
        
        addBullets();
    }
    
    void render(){
        // loop throug every enemy
        for (size_t i = 0; i < ene.size(); i++) ene[i].render();
    }
    
    void addBullets(){
        for (size_t i = 0; i < ene.size(); i++){
            if (rand() % 1000 < 5) ene[i].addBullet(bullet);
        }
    }
    

private:
    int numEn;
    int numCol;
    int numRow;
    
    glm::vec3 pos;
    glm::vec3 velo;
    glm::vec3 shape;
    
    XMLData bullet;
    
    class Enemy: public Object{
    public:
        vector<Bullet> bul;
        
        Enemy(GLuint texture, const XMLData& data, const glm::vec3& pos, const glm::vec3& velo): Object(&textured, texture, pos, velo){
            Object::setData(data);
        }
        
        void addBullet(const XMLData& bullet){
            if (bul.size() < 20){
                Bullet temp (texture, bullet, pos, glm::vec3(0, -0.7, 0));
                temp.setRotate(90 / 180 * M_PI);
                temp.setScale(0.5);
                bul.push_back(temp);
            }
        }
        
        void delBullet(int index){
            // remove the bullet when collide
        }
        
        void update(float elapsed){
            pos += elapsed * velo;
            Object::update();
            
            for (size_t i = 0; i < bul.size(); i++) bul[i].update(elapsed);
        }
        
        void render(){
            Object::render();
            for (size_t i = 0; i < bul.size(); i++) bul[i].render();
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
    
    void render(){
        
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

//    GLuint texture;
//    textured = setTextured("Asset/font1.png", texture);
//    Text disp(&textured, texture);
    
    GLuint texture;
    textured = setTextured("Asset/sheet.png", texture);

//    untextured = setUntextured();
    EnemyGroup ene(texture, xml.getData("playerShip2_orange.png"), xml.getData("laserBlue01.png"), glm::vec3(0));

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
        
        ene.update(elapsed);
        
        // display
        glClear(GL_COLOR_BUFFER_BIT);
        
        ene.render();

        SDL_GL_SwapWindow(displayWindow);
    }

    SDL_Quit();

    return 0;
}
