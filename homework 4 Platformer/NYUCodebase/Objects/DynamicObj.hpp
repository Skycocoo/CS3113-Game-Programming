// Yuxi Luo (yl4217), March 27, 2018
// CS3113 Game Programming

#ifndef DynamicObj_hpp
#define DynamicObj_hpp

#include "Object.h"
#include "StaticObj.hpp"

extern ShaderProgram textured;

class DynamicObj: public Object{
public:

    DynamicObj(): Object::Object(){}
    DynamicObj(GLuint texture, const glm::vec3& pos = glm::vec3(0, 0, 0), const glm::vec3& velo = glm::vec3(0, 0, 0)): Object(&textured, texture, pos), velo(velo), fric(glm::vec3(0.01, 0.01, 0.01)), grav(glm::vec3(0, -0.1, 0)){}

    // void setVelo(const glm::vec3& velo);
    // void setVelo(float x, float y, float z = 1.0);


    void updateVelo(float elapsed){
        lerp(velo, fric * elapsed);
        velo += acce * elapsed;
       // velo += grav * elapsed;
    }

    bool collide(float elapsed, const Object& rhs) {
        bool result = false;
        
        updateVelo(elapsed);

        // x axis:
        pos.x += velo.x * elapsed;
        result = Object::collide(rhs);

        // y axis:
        pos.y += velo.y * elapsed;
        result = result || Object::collide(rhs);

        Object::update();
        
        return result;
    }


protected:
    glm::vec3 velo;
    glm::vec3 acce;

    glm::vec3 fric;
    glm::vec3 grav;

    // linear interpolation
    void lerp(glm::vec3& orig, const glm::vec3& prop, const glm::vec3& tar = glm::vec3(0, 0, 0)){
        orig = (glm::vec3(1, 1, 1) - prop) * orig + prop * tar;
    }


};

#endif /* DynamicObj_hpp */
