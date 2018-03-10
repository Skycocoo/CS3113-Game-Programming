// Yuxi Luo (yl4217), March 3, 2018
// CS3113 Game Programming

#include "Bullet.h"

extern ShaderProgram untextured;
extern float screenHeight;

Bullet::Bullet(const glm::vec3& pos, const glm::vec3& velo): Object(&untextured, 0, pos, velo){
    Object::setShape(glm::vec3(0.05, 0.4, 0));
}

void Bullet::update(float elapsed){
    pos += elapsed * velo;
    Object::update();
}

void Bullet::render(bool player){
    if (player) program->SetColor(0 / 255.0, 199 / 255.0, 255 / 255.0, 1);
    Object::render();
}

bool Bullet::beyound(){
    return (pos.y < -screenHeight || pos.y > screenHeight);
}

