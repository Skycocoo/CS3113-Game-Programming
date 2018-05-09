// Yuxi Luo (yl4217), March 3, 2018
// CS3113 Game Programming

#ifndef Player_hpp
#define Player_hpp

#include "DynamicObj.hpp"

class EnemyGroup;

class Player: public DynamicObj{
public:
    int numJump;
    int points;
    bool end;
    bool superJump = false;

    Player();
    Player(ShaderProgram* program, GLuint texture, const std::vector<XMLData>& data, const glm::vec3& pos, const Tile* tile);

    void jump(float disp);
    void dead();

    void update(float elapsed);
    void updateState();

    bool collide(float elapsed, EnemyGroup& enemygroup);
    bool satCollide(float elapsed, EnemyGroup& enemygroup, Player& player);

private:
    std::vector<XMLData> textures;
    int lastState;
    float lastPos;
};

#endif /* Player_hpp */
