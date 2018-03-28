// Yuxi Luo (yl4217), March 3, 2018
// CS3113 Game Programming

#include "Enemy.h"

extern ShaderProgram textured;
extern ShaderProgram untextured;
extern float screenWidth;
extern float edge;

Enemy::Enemy(GLuint texture, const XMLData& data, const glm::vec3& pos, const glm::vec3& velo): DynamicObj(texture, pos, velo){
    Object::setData(data);
}

// update positiin
void Enemy::update(float elapsed){
    pos += elapsed * velo;
    Object::update();
}

// render enemy & bullets
void Enemy::render(){
    Object::render();
    untextured.SetColor(1, 1, 1, 1);
}

float Enemy::getX() const {
    return pos.x;
}


EnemyGroup::EnemyGroup(){}

EnemyGroup::EnemyGroup(GLuint texture, const XMLData& data, const glm::vec3& pos, const glm::vec3& velo, int numEn, int numCol): numEn(numEn), numCol(numCol), numRow(numEn/numCol), size(0.8), velo(velo){
    
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

void EnemyGroup::update(float elapsed){
    
    // beyound the range of the screenwidth
    float minX = 6, maxX = -6;
    for (size_t i = 0; i < ene.size(); i++){
        if (ene[i].getX() < minX) minX = ene[i].getX();
        else if (ene[i].getX() > maxX) maxX = ene[i].getX();
    }
    
//    // update position for enemies
//    // if approach to the edge : reverse sign of every velocity x
//    if ((((minX - size / 2 - edge) < -screenWidth) && (velo.x < 0))||(((maxX + size / 2 + edge) > screenWidth) && (velo.x > 0))){
//        velo.x = -velo.x;
//        for (size_t i = 0; i < ene.size(); i++) ene[i].setVelo(velo);
//    }
//
    for (size_t i = 0; i < ene.size(); i++) ene[i].update(elapsed);
}

// render enemies
void EnemyGroup::render(){
    for (size_t i = 0; i < ene.size(); i++) ene[i].render();
}

void EnemyGroup::delEne(size_t index){
    numEn -= 1;
    ene.erase(ene.begin() + index);
}

int EnemyGroup::getEne() const {
    return numEn;
}


