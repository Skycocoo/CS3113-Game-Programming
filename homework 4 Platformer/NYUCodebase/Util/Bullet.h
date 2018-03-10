// Yuxi Luo (yl4217), March 3, 2018
// CS3113 Game Programming

#ifndef Bullet_h
#define Bullet_h

#include "Object.h"

class Bullet: public Object{
public:
    Bullet(const glm::vec3& pos, const glm::vec3& velo = glm::vec3(0, 0.7, 0));
    
    void update(float elapsed);
    void render(bool player = false);
    
    bool beyound();
};

#endif /* Bullet_h */
