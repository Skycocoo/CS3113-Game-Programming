// Yuxi Luo (yl4217), March 27, 2018
// CS3113 Game Programming

#include "DynamicObj.hpp"
#include "Tile.hpp"

extern ShaderProgram textured;

DynamicObj::DynamicObj(): Object::Object(){}
DynamicObj::DynamicObj(GLuint texture, const glm::vec3& pos, const glm::vec3& velo):
    Object(&textured, texture, pos), velo(velo), fric(glm::vec3(0.01, 0.01, 0.01)), grav(glm::vec3(0, -0.5, 0)), acce(glm::vec3(0, 0, 0))
    {}

void DynamicObj::update(float elapsed){
    Object::update(elapsed);
}
void DynamicObj::render(const Matrix& view){
    Object::render(view);
}

void DynamicObj::updateVelo(float elapsed){
    lerp(velo, fric * elapsed);
    velo += acce * elapsed;
    velo += grav * elapsed;

    if (acce.x != 0) acce.x = 0;
//    if (acce.y != 0) acce.y = 0;
}

bool DynamicObj::collide(float elapsed, const Object& rhs) {
    bool result = false;
    updateVelo(elapsed);

    // x axis:
    pos.x += velo.x * elapsed;
    result = Object::collide(rhs);
    // should also check collide with tile map
    if (tile) result = result || tile->collide(*this);
    if (result) velo.x = 0;

    // y axis:
    pos.y += velo.y * elapsed;
    result = result || Object::collide(rhs);
    // should also check collide with tile map
    if (tile) result = result || tile->collide(*this);
    if (result) velo.y = 0;

    Object::update();

    return result;
}

bool DynamicObj::collide(float elapsed) {
    bool result = false;

//    std::cout << acce.x << " ";
    updateVelo(elapsed);

//    std::cout << velo.x << " " << velo.y << std::endl;

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
