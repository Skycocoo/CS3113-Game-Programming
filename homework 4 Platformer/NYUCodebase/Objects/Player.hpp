// Yuxi Luo (yl4217), March 3, 2018
// CS3113 Game Programming

#ifndef Player_hpp
#define Player_hpp

#include "DynamicObj.hpp"

class EnemyGroup;

class Player: public DynamicObj{
public:
    Player();
    Player(GLuint texture, const XMLData& data, const glm::vec3& pos, const Tile* tile);

    void control(float disp);
    void jump(float disp);

    bool collide(float elapsed, EnemyGroup& enemygroup);

};

#endif /* Player_hpp */
