// Yuxi Luo (yl4217), March 3, 2018
// CS3113 Game Programming

#include "Enemy.hpp"
#include "Tile.hpp"

extern ShaderProgram textured;
extern ShaderProgram untextured;
extern float screenWidth;
extern float edge;

Enemy::Enemy(){}

Enemy::Enemy(GLuint texture, const XMLData& data, const glm::vec3& pos, const Tile* tile): DynamicObj(texture, pos, tile){
    Object::setData(data);
}

// update positiin
void Enemy::update(float elapsed){
    pos += elapsed * velo;
    Object::update();
}

void Enemy::control(float disp){
    acce.x += disp;
}


bool Enemy::collide(float elapsed, EnemyGroup& enemygroup){
    bool x = false, y = false;
    updateVelo(elapsed);

    // x axis:
    pos.x += velo.x * elapsed;

    for (size_t i = 0; i < enemygroup.ene.size(); i++){
        if (this != &enemygroup.ene[i]){
            bool result = Object::collide(enemygroup.ene[i]);
            if (result){
                if (coll.left) enemygroup.ene[i].control(-5);
                if (coll.right) enemygroup.ene[i].control(5);
                x = true;
            }
        }
    }
    if (tile) x = tile->collide(*this);
    if (x) velo.x = 0;

    // std::cout << pos.x << " " << velo.x << std::endl;;

    // y axis:
    pos.y += velo.y * elapsed;

    for (size_t i = 0; i < enemygroup.ene.size(); i++){
        if (this != &enemygroup.ene[i]){
            y = Object::collide(enemygroup.ene[i]) || y;
        }
    }
    if (tile) y = tile->collide(*this) || y;
    if (y) velo.y = 0;

    Object::update();
    // std::cout << velo.x << " " << velo.y << std::endl;

    return (x || y);
}

bool Enemy::satCollide(float elapsed, EnemyGroup& enemygroup){
    // update x & y positions
    DynamicObj::update(elapsed);

    // store updated positions
    float prevX = pos.x, prevY = pos.y;
    bool result = false;

    // std::cout << "player" << std::endl;

    for (size_t i = 0; i < enemygroup.ene.size(); i++){
        // std::cout << "calling satCollide " << i << std::endl;
        if (this != &enemygroup.ene[i]){
            result = Object::satCollide(enemygroup.ene[i]) || result;
            if (coll.left) enemygroup.ene[i].setAcce(-5);
            else if (coll.right) enemygroup.ene[i].setAcce(5);
        }
    }

    if (tile) result = tile->collide(*this) || result;

    if (prevX - pos.x != 0) velo.x = 0;
    if (prevY - pos.y != 0) velo.y = 0;

    Object::update();
    return result;
}

// // render enemy & bullets
// void Enemy::render(){
//     Object::render();
//     // untextured.SetColor(1, 1, 1, 1);
// }

EnemyGroup::EnemyGroup(){}

EnemyGroup::EnemyGroup(GLuint texture, const XMLData& data, const glm::vec3& pos, const Tile* tile):
    size(1), numEn(3), numCol(3), numRow(1){
    // create enemy objects
    float posX = pos.x, posY = pos.y, spacing = 0.3;
    float step = size + spacing;

    for (int i = 0; i < numRow; i++){
        float relativeY = i - float(numRow - 1) / float(2);

        for (int j = 0; j < numCol; j++){
            float relativeX = j - float(numCol - 1) / float(2);
            // std::cout << posX + relativeX * step << " " << posY + relativeY * step << std::endl;
            Enemy temp (texture, data, glm::vec3(posX + relativeX * step, posY + relativeY * step, 0), tile);
            temp.setScale(size);
            ene.push_back(temp);
        }
    }
}

void EnemyGroup::setPos(const glm::vec3& pos){
    float posX = pos.x, posY = pos.y, spacing = 0.3;
    float step = size + spacing;

    for (size_t i = 0; i < ene.size(); i++){
        float relativeY = i - float(numRow - 1) / float(2);

        for (int j = 0; j < numCol; j++){
            float relativeX = j - float(numCol - 1) / float(2);
            ene[i].setPos(posX + relativeX * step, posY + relativeY * step);
            ene[i].setVelo(0, 0);
        }
    }
}

void EnemyGroup::setVelo(float x, float y){
    for (size_t i = 0; i < ene.size(); i++){
        ene[i].setVelo(x, y);
    }
}


void EnemyGroup::setProject(float proj){
    for (size_t i = 0; i < ene.size(); i++){
        ene[i].setProject(proj);
    }
}

void EnemyGroup::setScale(float scale){
    for (size_t i = 0; i < ene.size(); i++){
        ene[i].setScale(scale);
    }
}

bool EnemyGroup::collide(float elapsed){
    bool result = false;
    for (size_t i = 0; i < ene.size(); i++){
        result = ene[i].collide(elapsed, *this) || result;
    }
    return result;
}

bool EnemyGroup::satCollide(float elapsed){
    bool result = false;
    for (size_t i = 0; i < ene.size(); i++){
        result = ene[i].satCollide(elapsed, *this) || result;
    }
    return result;
}


// void EnemyGroup::update(float elapsed){
//     // beyound the range of the screenwidth
//     float minX = 6, maxX = -6;
//     for (size_t i = 0; i < ene.size(); i++){
//         if (ene[i].getX() < minX) minX = ene[i].getX();
//         else if (ene[i].getX() > maxX) maxX = ene[i].getX();
//     }
//
// //    // update position for enemies
// //    // if approach to the edge : reverse sign of every velocity x
// //    if ((((minX - size / 2 - edge) < -screenWidth) && (velo.x < 0))||(((maxX + size / 2 + edge) > screenWidth) && (velo.x > 0))){
// //        velo.x = -velo.x;
// //        for (size_t i = 0; i < ene.size(); i++) ene[i].setVelo(velo);
// //    }
// //
//     for (size_t i = 0; i < ene.size(); i++) ene[i].update(elapsed);
// }


// render enemies
void EnemyGroup::render(const Matrix& view){
    for (int i = 0; i < ene.size(); i++) ene[i].render(view);
}

void EnemyGroup::delEne(size_t index){
    numEn -= 1;
    ene.erase(ene.begin() + index);
}

int EnemyGroup::getEne() const {
    return numEn;
}
