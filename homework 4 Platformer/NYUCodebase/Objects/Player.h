// Yuxi Luo (yl4217), March 3, 2018
// CS3113 Game Programming

#ifndef Player_h
#define Player_h

#include "DynamicObj.hpp"


class Player: public DynamicObj{
private:
    // class Live: public Object{
    // public:
    //     Live();
    //     Live(GLuint texture, const XMLData& data);
    // };

public:
    Player();
    Player(GLuint texture, const XMLData& data, const glm::vec3& pos = glm::vec3(0, 4, 0));

    void control(float disp);
    // void update(float elapsed);
    // void render(const Matrix& view);
    
    // void renderLives();

    // int getLives() const;
    int getScore() const;

    // void decLives();
    void incScore(int s);

    const glm::vec3& center() const {
        return pos;
    }

private:
    // 3 2 1 0
    // int lives = 3;
    int score = 0;
    // Live live;
    // std::vector<XMLData> data;// bullets: disappear when collide
};

#endif /* Player_h */
