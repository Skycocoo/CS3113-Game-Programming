// Yuxi Luo (yl4217), March 27, 2018
// CS3113 Game Programming

#include "DynamicObj.hpp"
#include "Tile.hpp"

extern ShaderProgram textured;
extern ShaderProgram untextured;

DynamicObj::DynamicObj(): Object::Object(){}

DynamicObj::DynamicObj(GLuint texture, const glm::vec3& pos, const Tile* tile):
    Object(&textured, texture, pos), velo(0, 0, 0), fric(0.01, 0.01, 0.01), grav(0, -0.98, 0), acce(0, 0, 0), tile(tile)
    {
        if (texture == 0) program = &untextured;

    }

void DynamicObj::update(float elapsed){
    Object::update(elapsed);
}
void DynamicObj::render(const Matrix& view){
    Object::render(view);
}

const glm::vec3 DynamicObj::getVelo() const {
    return velo;
}

void DynamicObj::setVelo(float x, float y){
    velo.x = x;
    velo.y = y;
}

void DynamicObj::updateVelo(float elapsed){
    lerp(velo, fric * elapsed);
    velo += acce * elapsed;
    velo += grav * elapsed;

    if (acce.x != 0) acce.x = 0;
   // if (acce.y != 0) acce.y = 0;
}

bool DynamicObj::satCollide(float elapsed, const Object& rhs) {
    updateVelo(elapsed);
    pos.x += velo.x * elapsed;
    pos.y += velo.y * elapsed;

    float prevX = pos.x, prevY = pos.y;
    bool x = false, y = false;

    Object::update();
    bool result = Object::satCollide(rhs);
    if (tile) result = result || tile->collide(*this);

    if (result){
        if (prevY - pos.y != 0) y = true;
        if (prevX - pos.x != 0) x = true;
    }

    if (x) velo.x = 0;
    if (y) velo.y = 0;

    Object::update();
    return (x || y);
}


bool DynamicObj::collide(float elapsed, const Object& rhs) {
    bool x = false, y = false;
    updateVelo(elapsed);

    // x axis:
    pos.x += velo.x * elapsed;
    x = Object::collide(rhs);
    if (tile) x = x || tile->collide(*this);
    if (x) velo.x = 0;

    // y axis:
    pos.y += velo.y * elapsed;
    y = Object::collide(rhs);
    if (tile) y = y || tile->collide(*this);
    if (y) velo.y = 0;

    Object::update();
    return (x || y);
}

bool DynamicObj::collide(float elapsed) {
    bool result = false;
    updateVelo(elapsed);

    // x axis:
    pos.x += velo.x * elapsed;
    if (tile) result = result || tile->collide(*this);
    if (result) velo.x = 0;

    // y axis:
    pos.y += velo.y * elapsed;
    // should also check collide with tile map
    if (tile) result = result || tile->collide(*this);
    if (result) velo.y = 0;

    Object::update();

    return result;
}


// linear interpolation
void DynamicObj::lerp(glm::vec3& orig, const glm::vec3& prop, const glm::vec3& tar) const {
    orig = (glm::vec3(1, 1, 1) - prop) * orig + prop * tar;
}
