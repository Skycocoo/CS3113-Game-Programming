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


    void updateVelo(float elapsed){
        Object::lerp(velo, fric * elapsed);
        velo += acce * elapsed;
       // velo += grav * elapsed;
    }

    bool collide(float elapsed, const Object& rhs) {
        updateVelo(elapsed);

        // x axis:
        pos.x += velo.x * elapsed;

        // need to call object::collide to test actual collision (comparing only x axis is not enough)
        if (Object::collide(rhs)){
            if (pos.x - shape.x / 2 <= rhs.pos.x + rhs.shape.x / 2){
                collide.left = true;
            } else if (pos.x + shape.x / 2 >= rhs.pos.x - rhs.shape.x / 2){
                collide.right = true;
            }

            float penetration = fbas((pos.x - rhs.pos.x) - shape.x - rhs.shape.x);

            if (collide.left) pos.x += peneration + 0.0001;
            else pos.x -= peneration + 0.0001;
        }

        // y axis:
        pos.y += velo.y * elapsed;

        // need to call object::collide to test actual collision (comparing only x axis is not enough)
        if (Object::collide(rhs)){
            if (pos.y - shape.y / 2 <= rhs.pos.y + rhs.shape.y / 2){
                collide.bottom = true;
            } else if (pos.y + shape.y / 2 >= rhs.pos.y - rhs.shape.y / 2){
                collide.top = true;
            }

            float penetration = fbas((pos.y - rhs.pos.y) - shape.y - rhs.shape.y);

            if (collide.bottom) pos.y += peneration + 0.0001;
            else pos.y -= peneration + 0.0001;
        }

        Object::update();
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
