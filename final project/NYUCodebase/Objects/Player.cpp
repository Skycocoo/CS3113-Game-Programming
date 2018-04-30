// Yuxi Luo (yl4217), March 3, 2018
// CS3113 Game Programming

#include "Player.hpp"
#include "Enemy.hpp"
#include "Tile.hpp"

Player::Player(){}

Player::Player(GLuint texture, const XMLData& data, const glm::vec3& pos, const Tile* tile): DynamicObj(texture, pos, tile){
    Object::setData(data);
}



void Player::jump(float disp){
//    if (coll.bottom)
        velo.y += disp;
}

bool Player::collide(float elapsed, EnemyGroup& enemygroup){
    bool x = false, y = false;
    updateVelo(elapsed);
    // std::cout << "player" << std::endl;
    // x axis:
    pos.x += velo.x * elapsed;
    for (size_t i = 0; i < enemygroup.ene.size(); i++){
        bool result = Object::collide(enemygroup.ene[i]);
        if (result){
            if (coll.left) enemygroup.ene[i].setAcce(-5);
            if (coll.right) enemygroup.ene[i].setAcce(5);
            x = true;
        }
        // shouldnt update velocity for size() many times
        // enemygroup.ene[i].collide(elapsed, enemygroup);
    }
    if (tile) x = tile->collide(*this) || x;
    if (x) velo.x = 0;

    // y axis:
    pos.y += velo.y * elapsed;
    // should only push in x direction
    for (int i = 0; i < enemygroup.ene.size(); i++){
        y = Object::collide(enemygroup.ene[i]) || y;
        // shouldnt update velocity for size() many times
        // enemygroup.ene[i].collide(elapsed, enemygroup);
    }
    if (tile) y = tile->collide(*this) || y;
    if (y) velo.y = 0;

    // std::cout << std::boolalpha << x << " " << y << std::endl;

    Object::update();
    return (x || y);
}

bool Player::satCollide(float elapsed, EnemyGroup& enemygroup){
    // update x & y positions
    DynamicObj::update(elapsed);

    // store updated positions
    float prevX = pos.x, prevY = pos.y;
    bool result = false;

    // std::cout << "player" << std::endl;

    for (size_t i = 0; i < enemygroup.ene.size(); i++){
        // std::cout << "calling satCollide " << i << std::endl;
        result = Object::satCollide(enemygroup.ene[i]) || result;

        // if (result) std::cout << "collide? " << coll;

        if (coll.left) enemygroup.ene[i].setAcce(-5);
        else if (coll.right) enemygroup.ene[i].setAcce(5);
    }

    if (tile) result = tile->collide(*this) || result;

    if (prevX - pos.x != 0) velo.x = 0;
    if (prevY - pos.y != 0) velo.y = 0;

    Object::update();
    return result;
}
