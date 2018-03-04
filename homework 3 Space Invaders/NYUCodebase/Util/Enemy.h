// Yuxi Luo (yl4217), March 3, 2018
// CS3113 Game Programming

#ifndef Enemy_h
#define Enemy_h

#include "Object.h"
#include "Bullet.h"

class Enemy: public Object{
public:
    std::vector<Bullet> bul;
    
    Enemy(GLuint texture, const XMLData& data, const glm::vec3& pos, const glm::vec3& velo);
    
    void addBullet();
    void delBullet(size_t index);
    
    void update(float elapsed);
    void render();
    
    float getX() const;
};


class EnemyGroup{
public:
    std::vector<Enemy> ene;
    
    EnemyGroup();
    
    EnemyGroup(GLuint texture, const XMLData& data, const glm::vec3& pos, const glm::vec3& velo = glm::vec3(2, 0, 0), int numEn = 12, int numCol = 6);
    
    void update(float elapsed);
    void render();
    
    void addBullets();
    
    void delEne(size_t index);
    int getEne() const;
    
private:
    int numEn;
    int numCol;
    int numRow;
    
    float size;
    glm::vec3 velo;
};

#endif /* Enemy_h */
