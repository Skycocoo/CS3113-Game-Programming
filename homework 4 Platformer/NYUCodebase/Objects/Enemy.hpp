// Yuxi Luo (yl4217), March 3, 2018
// CS3113 Game Programming

#ifndef Enemy_hpp
#define Enemy_hpp

#include "DynamicObj.hpp"

class Enemy: public DynamicObj{
public:
    Enemy();
    Enemy(GLuint texture, const XMLData& data, const glm::vec3& pos, const Tile& tile);
};


class EnemyGroup{
public:
    std::vector<Enemy> ene;

    EnemyGroup();
    EnemyGroup(GLuint texture, const XMLData& data, const glm::vec3& pos, const Tile& tile);

    void update(float elapsed);
    void render();

    void delEne(size_t index);
    int getEne() const;

private:
    int numEn;
    // int numCol;
    // int numRow;

    float size;
    glm::vec3 velo;
};

#endif /* Enemy_hpp */
