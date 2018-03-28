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

Player::Player(GLuint texture, const std::vector<XMLData>& data, const glm::vec3& pos): Object(&textured, texture, pos), live(texture, data[data.size()-1]), data(data){
    Object::setData(data[0]);

}

void Player::control(float disp){
    acce.x += disp;
}

void Player::update(float elapsed){
    // apply friction
    Object::lerp(velo, fric * elapsed);
    velo += acce * elapsed;
   // velo += grav * elapsed;

    // check boundary
    if ((pos.x + shape.x / 2 <= screenWidth) && (pos.x - shape.x / 2 >= -screenWidth))  pos += velo * elapsed;
    else {
        Object::lerp(pos, glm::vec3(0.0001, 0, 0));
        velo.x = 0;
    }

    if (acce.x != 0) acce.x = 0;

    Object::update();
}

void Player::render(){
    Object::render();
}

void Player::renderLives(){
    for (int i = 0; i < lives; i++){
        live.setPos(glm::vec3(4.5 + 0.5 * i, 3.5, 0));
        live.update();
        live.render();
    }
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
