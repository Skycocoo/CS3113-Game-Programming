// Yuxi Luo (yl4217), March 3, 2018
// CS3113 Game Programming

#include "Player.h"

extern ShaderProgram textured;
extern float screenWidth;

Player::Live::Live(){}

Player::Live::Live(GLuint texture, const XMLData& data): Object(&textured, texture){
    Object::setData(data);
    Object::setScale(0.5);
}



Player::Player(){}

Player::Player(GLuint texture, const std::vector<XMLData>& data, glm::vec3 pos): Object(&textured, texture, pos), live(texture, data[data.size()-1]), data(data){
    Object::setData(data[0]);
    
}

void Player::control(float distance){
    if ((pos.x + distance + shape.x / 2 < screenWidth) && (pos.x + distance - shape.x / 2 > -screenWidth)) pos.x += distance;
}

void Player::update(float elapsed){
    Object::update();
    for (size_t i = 0; i < bul.size(); i++) {
        bul[i].update(elapsed);
        if (bul[i].beyound()) delBullet(i);
    }
}

void Player::render(){
    Object::render();
    for (size_t i = 0; i < bul.size(); i++) bul[i].render(true);
}

void Player::renderLives(){
    for (int i = 0; i < lives; i++){
        live.setPos(glm::vec3(4.5 + 0.5 * i, 3.5, 0));
        live.update();
        live.render();
    }
}

// max amount of bullets: 10
void Player::addBullet(){
    
    // bullet from current position
    if (bul.size() < 10) bul.push_back(Bullet(glm::vec3(pos.x, pos.y + shape.y / 2, 0), glm::vec3(0, 3, 0)));
    else return;
}


// remove the bullet when collide
void Player::delBullet(size_t index){
    
    // bul.erase(bul.begin() + index);
    bul[index] = bul[bul.size() - 1];
    bul.pop_back();
}



int Player::getLives() const {
    return lives;
}

int Player::getScore() const {
    return score;
}

void Player::decLives(){
    lives -= 1;
    Object::setData(data[3 - lives]);
}

void Player::incScore(int s){
    score += s;
}

