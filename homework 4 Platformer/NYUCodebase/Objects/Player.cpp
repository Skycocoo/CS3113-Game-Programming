// Yuxi Luo (yl4217), March 3, 2018
// CS3113 Game Programming

#include "Player.hpp"
#include "Enemy.hpp"
#include "Tile.hpp"

Player::Player(){}

Player::Player(GLuint texture, const XMLData& data, const glm::vec3& pos, const Tile* tile): DynamicObj(texture, pos, tile){
    Object::setData(data);
}

void Player::control(float disp){
    acce.x += disp;
}

void Player::jump(float disp){
//    if (coll.bottom)
        velo.y += disp;
}

bool Player::collide(float elapsed, EnemyGroup& enemygroup){
    // need to fix boolean
    bool x = false, y = false;
    updateVelo(elapsed);

    // x axis:
    pos.x += velo.x * elapsed;
    for (int i = 0; i < enemygroup.ene.size(); i++){
        bool result = Object::collide(enemygroup.ene[i]);
        if (result){
            if (coll.left) enemygroup.ene[i].control(-5);
            if (coll.right) enemygroup.ene[i].control(5);
            x = true;
        }
        // shouldnt update velocity for size() many times
        // enemygroup.ene[i].collide(elapsed, enemygroup);
    }
    if (tile) x = x || tile->collide(*this);
    if (x) velo.x = 0;

    // y axis:
    pos.y += velo.y * elapsed;
    // should only push in x direction
    for (int i = 0; i < enemygroup.ene.size(); i++){
        y = y || Object::collide(enemygroup.ene[i]);
        // shouldnt update velocity for size() many times
        // enemygroup.ene[i].collide(elapsed, enemygroup);
    }
    if (tile) y = y || tile->collide(*this);
    if (y) velo.y = 0;

    Object::update();
    return (x || y);
}
