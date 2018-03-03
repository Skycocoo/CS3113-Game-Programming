// Yuxi Luo (yl4217), February 26, 2018
// Homework 3, Space Invaders, CS3113 Game Programming

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <cstdlib>
#include <vector>

#include "Util/Util.h"
#include "setUp.h"

using namespace std;


float screenRatio = 0.0, screenHeight = 0.0, screenWidth = 0.0, splitScale = 0.0, edge = 1.0;

ShaderProgram textured;
ShaderProgram untextured;


enum GameMode{STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER};


class Bullet: public Object{
public:
    
    Bullet(const glm::vec3& pos, const glm::vec3& velo = glm::vec3(0, 0.7, 0)): Object(&untextured, 0, pos, velo){
        Object::setShape(glm::vec3(0.05, 0.4, 0));
    }
    
    void update(float elapsed){
        pos += elapsed * velo;
        Object::update();
    }
    
    void render(bool player = false){
        if (player) program->SetColor(13.0/255.0, 132.0/255.0, 1, 1);
        Object::render();
    }
    
    bool beyound(){
        return (pos.y < -screenHeight || pos.y > screenHeight);
    }
    
    //    Bullet(GLuint texture, const XMLData& data, const glm::vec3& pos, const glm::vec3& velo = glm::vec3(0, 0.7, 0)): Object(&textured, texture, pos, velo){
    //        Object::setShape(glm::vec3(0.1, 0.1, 0));
    //        Object::setData(data);
    //    }
    
};

class Player: public Object{
public:
    vector<Bullet> bul;
    
    Player(GLuint texture, const vector<XMLData>& data, const XMLData& bullet, glm::vec3 pos = glm::vec3(0, -3, 0)): Object(&textured, texture, pos), data(data), bullet(bullet){
        Object::setData(data[0]);
    }

    void control(float distance){
        if ((pos.x + distance + shape.x / 2 < screenWidth) && (pos.x + distance - shape.x / 2 > -screenWidth)) pos.x += distance;
    }
    
    void update(float elapsed){
        Object::update();
        for (size_t i = 0; i < bul.size(); i++) {
            bul[i].update(elapsed);
            if (bul[i].beyound()) delBullet(i);
        }
    }
    
    void render(){
        Object::render();
        
        for (size_t i = 0; i < bul.size(); i++) bul[i].render(true);
    }
    
    // max amount of bullets: 20
    void addBullet(){
        // bullet from current position
        if (bul.size() < 20) bul.push_back(Bullet(glm::vec3(pos.x, pos.y + shape.y / 2, 0)));
    }
    
    // remove the bullet when collide
    void delBullet(size_t index){
        // bul.erase(bul.begin() + index);
        bul[index] = bul[bul.size() - 1];
        bul.pop_back();
    }
    
    int getLives() const {
        return lives;
        
    }
    
    void decLives(){
        lives -= 1;
        Object::setData(data[4 - lives]);
    }

private:
    int lives = 4;
    vector<XMLData> data;
    
    XMLData bullet;
    
};



class Enemy: public Object{
public:
    vector<Bullet> bul;
    
    Enemy(GLuint texture, const XMLData& data, const glm::vec3& pos, const glm::vec3& velo): Object(&textured, texture, pos, velo){
        Object::setData(data);
    }
    
    // add bullets
    void addBullet(const XMLData& bullet){
        if (bul.size() < 3){
            bul.push_back(Bullet(glm::vec3(pos.x, pos.y - shape.y / 2, 0), glm::vec3(0, -0.7, 0)));
            
            // // decide not to use texture for bullets
            // Bullet temp (texture, bullet, glm::vec3(pos.x, pos.y - shape.y / 2, 0), glm::vec3(0, -0.7, 0));
            // temp.setRotate(90 / 180 * M_PI);
        }
    }
    
    // remove the bullet when collide
    void delBullet(size_t index){
        bul.erase(bul.begin() + index);
    }
    
    // update positiin
    void update(float elapsed){
        pos += elapsed * velo;
        Object::update();
        
        for (size_t i = 0; i < bul.size(); i++) {
            bul[i].update(elapsed);
            if (bul[i].beyound()) delBullet(i);
        }
    }
    
    // render enemy & bullets
    void render(){
        Object::render();
        untextured.SetColor(1, 1, 1, 1);
        for (size_t i = 0; i < bul.size(); i++) bul[i].render();
    }
    
};


class EnemyGroup{
public:
    vector<Enemy> ene;
    
    // enemy group don't use shaderprogram itself
    // shaderprogram & texture & data is for enemy
    // need to restructure enemygroup
    EnemyGroup(GLuint texture, const XMLData& data, const XMLData& bullet, const glm::vec3& pos, const glm::vec3& velo = glm::vec3(2, 0, 0), int numEn = 12, int numCol = 6): numEn(numEn), numCol(numCol), numRow(numEn/numCol), pos(pos), velo(velo), bullet(bullet){
        
        // create enemy objects
        float posX = pos.x, posY = pos.y, size = 0.8, spacing = 0.3;
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
        shape = glm::vec3(4 * step + size, 2 * step + size, 0);
    }
    
    void update(float elapsed){
        // update position for enemy group
        pos += elapsed * velo;
        
        // add bullets for enemy
        addBullets();
        
        // update position for enemies
        // if approach to the edge : reverse sign of every velocity x
        if (((pos.x - shape.x / 2 - edge < -screenWidth) && (velo.x < 0))||((pos.x + shape.x / 2 + edge > screenWidth) && (velo.x > 0))){
            velo.x = -velo.x;
            for (size_t i = 0; i < ene.size(); i++) ene[i].setVelo(velo);
        }
        
        for (size_t i = 0; i < ene.size(); i++) ene[i].update(elapsed);
    }
    
    // render enemies
    void render(){
        for (size_t i = 0; i < ene.size(); i++) ene[i].render();
    }
    
    // add bullets
    void addBullets(){
        if (rand() % 100 < 1) ene[rand() % ene.size()].addBullet(bullet);
    }
    
    void delEne(size_t index){
        ene.erase(ene.begin() + index);
    }

private:
    int numEn;
    int numCol;
    int numRow;
    
    glm::vec3 pos;
    glm::vec3 velo;
    glm::vec3 shape;
    
    XMLData bullet;
};


class GameState{
public:
    GameState(Player& player, EnemyGroup& enemygroup): player(&player), enemygroup(&enemygroup){}
    
    // bullets: disappear when collide
    void checkCollision(){
        
        // players' bullet: collide with enemy / with board
        for (size_t i = 0; i < player->bul.size(); i++){
            for (size_t j = 0; j < enemygroup->ene.size(); j++){
                if (player->bul[i].collide(enemygroup->ene[j])){
                    player->delBullet(i);
                    enemygroup->delEne(j);
                }
            }
        }
        
        // enemy's bullet: collide with player / with board
        for (size_t i = 0; i < enemygroup->ene.size(); i++){
            for (size_t j = 0; j < enemygroup->ene[i].bul.size(); j++){
                if (enemygroup->ene[i].bul[j].collide(*player)){
                    player->decLives();
                    enemygroup->ene[i].delBullet(j);
                }
            }
        }
    }
    
    void update(float elapsed){
        // player, enemygroup update
        // collision detection
        
        checkCollision();
        
        player->update(elapsed);
        enemygroup->update(elapsed);
    }
    
    void render(){
        player->render();
        enemygroup->render();
    }
    
private:
    Player* player;
    EnemyGroup* enemygroup;
};


void displayGame(Text& disp){
    disp.render("Space Invaders", 0.8, 1, 0, 4.5);
}

void updatePlayer(const SDL_Event& event, Player& play){
    switch (event.type){
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode){
                case SDL_SCANCODE_SPACE:
                    play.addBullet();
                    break;
                case SDL_SCANCODE_LEFT:
                    play.control(-1);
                    break;
                case SDL_SCANCODE_RIGHT:
                    play.control(1);
                    break;
            }
            break;
    }
}



int main(){
    // initial set up
    srand(time(NULL));
    SDL_Window* displayWindow = setUp("Homework 3 Space Invaders");

    XMLLoad xml ("Asset/sheet.xml");
    untextured = setUntextured();

    GLuint text;
    textured = setTextured("Asset/font1.png", text);
    Text disp(&textured, text);
    
    GLuint texture;
    textured = setTextured("Asset/sheet.png", texture);
    
    vector<XMLData> playerlife;
    playerlife.push_back(xml.getData("playerShip1_blue.png"));
    playerlife.push_back(xml.getData("playerShip1_damage1.png"));
    playerlife.push_back(xml.getData("playerShip1_damage2.png"));
    playerlife.push_back(xml.getData("playerShip1_damage2.png"));
    
    EnemyGroup ene(texture, xml.getData("enemyBlack1.png"), xml.getData("laserRed13.png"), glm::vec3(0, 2, 0));
    Player play(texture, playerlife, xml.getData("laserBlue13.png"), glm::vec3(0, -4, 0));

    GameState game(play, ene);
    
    
    SDL_Event event;
    bool done = false;
    float lastFrameTicks = 0.0f;
    while (!done) {
        // keyboard event
        while (SDL_PollEvent(&event)) {
            checkKeyboard(event, done);
            updatePlayer(event, play);
        }
        
        // update parameters
        float ticks = (float)SDL_GetTicks()/1000.0f;
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        
        game.update(elapsed);
        
        // display
        glClear(GL_COLOR_BUFFER_BIT);

        game.render();g
        displayGame(disp);

        SDL_GL_SwapWindow(displayWindow);
    }

    SDL_Quit();

    return 0;
}
