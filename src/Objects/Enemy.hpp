// Yuxi Luo (yl4217), March 3, 2018
// CS3113 Game Programming

#ifndef Enemy_hpp
#define Enemy_hpp

#include "DynamicObj.hpp"

class EnemyGroup;

class Enemy: public DynamicObj{
public:
    Enemy();
    Enemy(ShaderProgram* program, GLuint texture, const std::vector<XMLData>& data, const glm::vec3& pos, const Tile* tile);

    void update(float elapsed);
    void updateState();

    void control(float disp);

    bool collide(float elapsed, EnemyGroup& enemygroup);
    bool satCollide(float elapsed, EnemyGroup& enemygroup);

private:
    std::vector<XMLData> textures;
    int lastState;
    float lastPos;

    bool dead;
};


class EnemyGroup{
public:
    std::vector<Enemy> ene;

    EnemyGroup();
    EnemyGroup(ShaderProgram* program, GLuint texture, const std::vector<XMLData>& data, const glm::vec3& pos, const Tile* tile);

    // void update(float elapsed);
    void render(const Matrix& view = Matrix());
    bool collide(float elapsed);
    bool satCollide(float elapsed);

    void setShader(ShaderProgram* program);
    void setScale(float scale);
    void setPos(const glm::vec3& pos);
    void setVelo(float x, float y);
    void setProject(float proj);

    void delEne(size_t index);
    int getEne() const;

private:
    float size;

    int numEn;
    int numCol;
    int numRow;
};


#endif /* Enemy_hpp */
