// Yuxi Luo (yl4217), March 27, 2018
// CS3113 Game Programming

#ifndef DynamicObj_hpp
#define DynamicObj_hpp

#include "Object.h"
#include "StaticObj.hpp"

class DynamicObj: public Object{
public:

    DynamicObj(): Object::Object(){}
    DynamicObj(GLuint texture, const glm::vec3& pos = glm::vec3(0, 0, 0), const glm::vec3& velo = glm::vec3(0, 0, 0)): Object(&textured, texture, pos), velo(velo), , fric(glm::vec3(0.01, 0.01, 0.01)), grav(glm::vec3(0, -0.1, 0)){}

    // void setVelo(const glm::vec3& velo);
    // void setVelo(float x, float y, float z = 1.0);

    void collideHandle(const StaticObj& rhs){

    }

protected:
    // collision flags for four sides
    struct Coll{
        bool top = false, bottom = false, left = false, right = false;
    };

    glm::vec3 velo;
    glm::vec3 acce;

    glm::vec3 fric;
    glm::vec3 grav;

    Coll collide;

    // linear interpolation
    void lerp(glm::vec3& orig, const glm::vec3& prop, const glm::vec3& tar = glm::vec3(0, 0, 0)){
        orig = (glm::vec3(1, 1, 1) - prop) * orig + prop * tar;
    }


};

#endif /* DynamicObj_hpp */
