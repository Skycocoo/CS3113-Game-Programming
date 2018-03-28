// Yuxi Luo (yl4217), March 27, 2018
// CS3113 Game Programming

#ifndef StaticObj_hpp
#define StaticObj_hpp

#include "Object.h"

// no updates / gravity applied
class StaticObj: public Object{
public:
    StaticObj();
    StaticObj(GLuint texture, const glm::vec3& pos);

    // void update(float elapsed);
    // void render();
};



#endif /* StaticObj_hpp */
