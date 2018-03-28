// Yuxi Luo (yl4217), March 3, 2018
// CS3113 Game Programming

#include "Player.hpp"

Player::Player(){}

Player::Player(GLuint texture, const XMLData& data, const glm::vec3& pos): DynamicObj(texture, pos){
    Object::setData(data);

}

void Player::control(float disp){
    acce.x += disp;
}

int Player::getScore() const {
    return score;
}

void Player::incScore(int s){
    score += s;
}


//Player::Live::Live(){}
//
//Player::Live::Live(GLuint texture, const XMLData& data): Object(&textured, texture){
//    Object::setData(data);
//    Object::setScale(0.5);
//}

// void Player::renderLives(){
//     for (int i = 0; i < lives; i++){
//         live.setPos(glm::vec3(4.5 + 0.5 * i, 3.5, 0));
//         live.update();
//         live.render();
//     }
// }
//

// int Player::getLives() const {
//     return lives;
// }

// void Player::decLives(){
//     lives -= 1;
//     Object::setData(data[3 - lives]);
// }
