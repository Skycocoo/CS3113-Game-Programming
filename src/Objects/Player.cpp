// Yuxi Luo (yl4217), March 3, 2018
// CS3113 Game Programming

#include "Player.hpp"
#include "Enemy.hpp"
#include "Tile.hpp"

extern glm::vec3 startPos;

Player::Player(){}

// 0: original; 1: jump; 2: stand; 3: walk1; 4: walk2
Player::Player(ShaderProgram* program, GLuint texture, const std::vector<XMLData>& data, const glm::vec3& pos, const Tile* tile):
DynamicObj(program, texture, pos, tile), numJump(0), points(0), end(false), textures(data), lastState(0), lastPos(-100.0){
    Object::setData(textures[0]);
}

void Player::update(float elapsed){
    modelMatrix.Identity();

    modelMatrix.Translate(pos.x, pos.y, pos.z);
    modelMatrix.Rotate(rotate);

    if (velo.x < 0){
        modelMatrix.Scale(-scale, scale, scale);
    } else modelMatrix.Scale(scale, scale, scale);
}

void Player::updateState(){
    int state = 0;

    // in the air
    if (velo.y != 0) state = 1;
    else if (velo.x != 0) {
        // on the tile
        if (lastPos == -100){
            lastPos = pos.x;
            state = 3;
        }
        if (lastState == 3) {
            if (fabs(pos.x - lastPos) > 0.2){
                state = 4;
                lastPos = pos.x;
            } else state = 3;
        } else {
            if (fabs(pos.x - lastPos) > 0.2){
                state = 3;
                lastPos = pos.x;
            } else state = 4;
        }
    }
    lastState = state;
    Object::setData(textures[state]);
}



void Player::jump(float disp){
//    if (coll.bottom)
        velo.y += disp;
}

void Player::dead(){
    points -= 10;
    this->setPos(startPos);
    this->setVelo(0, 0);
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
            if (coll.left) enemygroup.ene[i].setVelo(-0.3);
            if (coll.right) enemygroup.ene[i].setVelo(0.3);
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

bool Player::satCollide(float elapsed, EnemyGroup& enemygroup, Player& player){
    // update x & y positions
    DynamicObj::update(elapsed);

    // store updated positions
    float prevX = pos.x, prevY = pos.y;
    bool result = false;
    bool bottom = false;

    // std::cout << "player" << std::endl;

    for (size_t i = 0; i < enemygroup.ene.size(); i++){
        // std::cout << "calling satCollide " << i << std::endl;
        result = Object::satCollide(enemygroup.ene[i]) || result;

        // if (result) std::cout << "collide? " << coll;

        if (coll.left) enemygroup.ene[i].setVelo(-0.3);
        else if (coll.right) enemygroup.ene[i].setVelo(0.3);

        if (coll.bottom) bottom = true;
    }

    result = Object::satCollide(player) || result;
    if (coll.left) player.setVelo(-0.3);
    else if (coll.right) player.setVelo(0.3);
    if (coll.bottom) bottom = true;


    if (tile) result = tile->collide(*this) || result;
    if ((prevX - pos.x != 0) || (coll.left || coll.right)) velo.x = 0;
    if ((prevY - pos.y != 0) || (coll.top || coll.bottom)) velo.y = 0;
    if (coll.bottom) bottom = true;

    if (bottom) this->numJump = 0;

    Player::updateState();
    Player::update(elapsed);
    return result;
}
