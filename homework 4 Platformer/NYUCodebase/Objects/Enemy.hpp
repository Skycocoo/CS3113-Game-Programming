// Yuxi Luo (yl4217), March 3, 2018
// CS3113 Game Programming

#ifndef Enemy_hpp
#define Enemy_hpp

#include "DynamicObj.hpp"

class EnemyGroup;

class Enemy: public DynamicObj{
public:
    Enemy();
    Enemy(GLuint texture, const XMLData& data, const glm::vec3& pos, const Tile& tile);

    void update(float elapsed);
    void control(float disp);

    bool collide(float elapsed, const EnemyGroup& enemygroup);
};


class EnemyGroup{
public:
    std::vector<Enemy> ene;

    EnemyGroup();
    EnemyGroup(GLuint texture, const XMLData& data, const glm::vec3& pos, const Tile& tile);

    void update(float elapsed);
    void render(const Matrix& view = Matrix());

    void setPos(const glm::vec3& pos);

    void delEne(size_t index);
    int getEne() const;

private:
    float size;

    int numEn;
    int numCol;
    int numRow;
};


#endif /* Enemy_hpp */
