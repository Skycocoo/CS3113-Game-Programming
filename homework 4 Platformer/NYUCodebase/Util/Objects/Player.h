// Yuxi Luo (yl4217), March 3, 2018
// CS3113 Game Programming

#ifndef Player_h
#define Player_h

#include "Object.h"
#include "Bullet.h"

class Player: public Object{
private:
    class Live: public Object{
    public:
        Live();
        Live(GLuint texture, const XMLData& data);
    };
    
public:
    std::vector<Bullet> bul;
    
    Player();
    Player(GLuint texture, const std::vector<XMLData>& data, glm::vec3 pos = glm::vec3(0, -3, 0));
    
    void control(float distance);
    void update(float elapsed);
    void render();
    void renderLives();
    

    void addBullet();
    void delBullet(size_t index);
    
    int getLives() const;
    int getScore() const;
    
    void decLives();
    void incScore(int s);
    
private:
    // 3 2 1 0
    int lives = 3;
    int score = 0;
    Live live;
    std::vector<XMLData> data;// bullets: disappear when collide
};

#endif /* Player_h */
