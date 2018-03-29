// Yuxi Luo (yl4217), March 3, 2018
// CS3113 Game Programming

#ifndef Player_hpp
#define Player_hpp

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
    Player(GLuint texture, const XMLData& data, const glm::vec3& pos = glm::vec3(0, 0, 0), const Tile& tile = Tile());

    void control(float disp);
    void jump(float disp);

    // int getScore() const;
    // void incScore(int s);

    // void renderLives();
    // int getLives() const;
    // void decLives();

    // ~Player(){
    //     std::cout << "~Player()\n";
    // }

private:
    // int score = 0;

    // 3 2 1 0
    // int lives = 3;
    // Live live;
    // std::vector<XMLData> data;// bullets: disappear when collide
};

#endif /* Player_hpp */
