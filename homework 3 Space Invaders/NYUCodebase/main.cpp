// Yuxi Luo (yl4217), February 26, 2018
// Homework 3, Space Invaders, CS3113 Game Programming

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <cstdlib>
#include <vector>

#include "Util/Util.h"
#include "setUp.h"

using namespace std;

enum GameMode{STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER};

GameMode mode = STATE_MAIN_MENU;
float screenRatio = 0.0, screenHeight = 0.0, screenWidth = 0.0, splitScale = 0.0, edge = 1.0;

ShaderProgram textured;
ShaderProgram untextured;


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
        if (player) program->SetColor(0 / 255.0, 199 / 255.0, 255 / 255.0, 1);
        Object::render();
    }
    
    bool beyound(){
        return (pos.y < -screenHeight || pos.y > screenHeight);
    }
};

class Player: public Object{
    
private:
    class Live: public Object{
    public:
        Live(){}
        Live(GLuint texture, const XMLData& data): Object(&textured, texture){
            Object::setData(data);
            Object::setScale(0.5);
        }
    };
    
public:
    vector<Bullet> bul;
    
    Player(){}
    Player(GLuint texture, const vector<XMLData>& data, glm::vec3 pos = glm::vec3(0, -3, 0)): Object(&textured, texture, pos), live(texture, data[data.size()-1]), data(data){
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
    
    void renderLives(){
        for (int i = 0; i < lives; i++){
            live.setPos(glm::vec3(4.5 + 0.5 * i, 3.5, 0));
            live.update();
            live.render();
        }
    }
    
    // max amount of bullets: 20
    void addBullet(){
        // bullet from current position
        if (bul.size() < 20) bul.push_back(Bullet(glm::vec3(pos.x, pos.y + shape.y / 2, 0), glm::vec3(0, 3, 0)));
        else return;
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
    
    int getScore() const {
        return score;
    }
    
    void decLives(){
        lives -= 1;
        Object::setData(data[3 - lives]);
    }
    
    void incScore(int s){
        score += s;
    }

private:
    // 3 2 1 0
    int lives = 3;
    int score = 0;
    Live live;
    vector<XMLData> data;// bullets: disappear when collide
    

};



class Enemy: public Object{
public:
    vector<Bullet> bul;
    
    Enemy(GLuint texture, const XMLData& data, const glm::vec3& pos, const glm::vec3& velo): Object(&textured, texture, pos, velo){
        Object::setData(data);
    }
    
    // add bullets
    void addBullet(){
        if (bul.size() < 3) bul.push_back(Bullet(glm::vec3(pos.x, pos.y - shape.y / 2, 0), glm::vec3(0, -2, 0)));
        else return;
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
    
    float getX() const {
        return pos.x;
    }
    
};


class EnemyGroup{
public:
    vector<Enemy> ene;

    EnemyGroup(){}
    
    EnemyGroup(GLuint texture, const XMLData& data, const glm::vec3& pos, const glm::vec3& velo = glm::vec3(2, 0, 0), int numEn = 12, int numCol = 6): numEn(numEn), numCol(numCol), numRow(numEn/numCol), size(0.8), velo(velo){
        
        // create enemy objects
        float posX = pos.x, posY = pos.y, spacing = 0.3;
        float step = size + spacing;
        
        for (int i = 0; i < numRow; i++){
            float relativeY = i - float(numRow - 1) / float(2);
            
            for (int j = 0; j < numCol; j++){
                float relativeX = j - float(numCol - 1) / float(2);
                Enemy temp (texture, data, glm::vec3(posX + relativeX * step, posY + relativeY * step, 0), velo);
                temp.setShape(glm::vec3(size));
                ene.push_back(temp);
            }
        }
    }
    
    void update(float elapsed){
        // add bullets for enemy
        addBullets();
        
        // beyound the range of the screenwidth
        float minX = 6, maxX = -6;
        for (size_t i = 0; i < ene.size(); i++){
            if (ene[i].getX() < minX) minX = ene[i].getX();
            else if (ene[i].getX() > maxX) maxX = ene[i].getX();
        }
        
        // update position for enemies
        // if approach to the edge : reverse sign of every velocity x
        if ((((minX - size / 2 - edge) < -screenWidth) && (velo.x < 0))||(((maxX + size / 2 + edge) > screenWidth) && (velo.x > 0))){
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
        if (rand() % 100 < 1) ene[rand() % ene.size()].addBullet();
        else return;
    }
    
    void delEne(size_t index){
        numEn -= 1;
        ene.erase(ene.begin() + index);
    }
    
    int getEne() const {
        return numEn;
    }

private:
    int numEn;
    int numCol;
    int numRow;
    
    float size;
    glm::vec3 velo;
};


class GameState{
public:
    
    Player player;
    EnemyGroup enemygroup;
    
    GameState(){
        xml = XMLLoad("Asset/sheet.xml");
        untextured = setUntextured();
        
        GLuint text;
        textured = setTextured("Asset/font1.png", text);
        disp = Text(&textured, text);
      
        init();
    }
    
    void init(){
        GLuint texture;
        textured = setTextured("Asset/sheet.png", texture);
        
        vector<XMLData> playerlife;
        playerlife.push_back(xml.getData("playerShip1_blue.png"));
        playerlife.push_back(xml.getData("playerShip1_damage1.png"));
        playerlife.push_back(xml.getData("playerShip1_damage2.png"));
        playerlife.push_back(xml.getData("playerShip1_damage3.png"));
        playerlife.push_back(xml.getData("playerLife1_blue.png"));
        
        enemygroup = EnemyGroup(texture, xml.getData("enemyBlack1.png"), glm::vec3(0, 2, 0));
        player = Player(texture, playerlife, glm::vec3(0, -4, 0));
    }
    
    
    // bullets: disappear when collide
    void checkCollision(){
        
        // players' bullet: collide with enemy / with board
        for (size_t i = 0; i < player.bul.size(); i++){
            for (size_t j = 0; j < enemygroup.ene.size(); j++){
                if (player.bul[i].collide(enemygroup.ene[j])){
                    player.delBullet(i);
                    player.incScore(10);
                    enemygroup.delEne(j);
                }
            }
        }
        
        // enemy's bullet: collide with player / with board
        for (size_t i = 0; i < enemygroup.ene.size(); i++){
            for (size_t j = 0; j < enemygroup.ene[i].bul.size(); j++){
                if (enemygroup.ene[i].bul[j].collide(player)){
                    player.decLives();
                    enemygroup.ene[i].delBullet(j);
                }
            }
        }
    }
    
    void update(float elapsed){
        switch (mode){
            case STATE_MAIN_MENU:
                break;
            case STATE_GAME_LEVEL:
                checkCollision();
                player.update(elapsed);
                enemygroup.update(elapsed);
                if (player.getLives() == 0 || enemygroup.getEne() == 0) mode = STATE_GAME_OVER;
                break;
            case STATE_GAME_OVER:
                break;
        }
    }
    
    
    void render(){
        switch (mode){
            case STATE_MAIN_MENU:
                displayMainMenu();
                break;
            case STATE_GAME_LEVEL:
                displayLevel();
                break;
            case STATE_GAME_OVER:
                displayOver();
                break;
        }

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
        
        disp.render("Score: " + to_string(player.getScore()), 0.4, 1, -4, 3.5);
        disp.render("Lives: ", 0.4, 1, 3.5, 3.5);
        player.renderLives();
    }
    
    void displayOver(){
        disp.render("Space Invaders", 1, 2, 0, 3.5);
        disp.render("Game Over", 1, 2, 0, 2);
        disp.render("B: begin   Q: quit", 0.5, 1, 0, -1.5);
    }
    
    
private:
    XMLLoad xml;
    Text disp;
};


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



    GameState game;
    
    
    SDL_Event event;
    bool done = false;
    float lastFrameTicks = 0.0f;
    while (!done) {
        // keyboard event
        while (SDL_PollEvent(&event)) {
            checkKeyboard(event, done);
            updateGame(event, game);
        }
        
        // update parameters
        float ticks = (float)SDL_GetTicks()/1000.0f;
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        
        game.update(elapsed);
        
        // display
        glClear(GL_COLOR_BUFFER_BIT);

        game.render();

        SDL_GL_SwapWindow(displayWindow);
    }

    SDL_Quit();

    return 0;
}
