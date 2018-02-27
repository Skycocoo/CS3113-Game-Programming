// Yuxi Luo (yl4217), February 15, 2018
// Homework 2, PONG!, CS3113 Game Programming

#ifndef Object_h
#define Object_h

#include <vector>
#include <string>
#include "ShaderProgram.h"
#include "../Math/Matrix.h"


// create an object class to handle parameters
class Object{
public:
    // matrices
    Matrix projectionMatrix;
    Matrix modelMatrix;
    Matrix viewMatrix;
    
    // positions
    float x = 0;
    float y = 0;
    float width = 1;
    float height = 1;
    float velocity_x = 0;
    float velocity_y = 0;
    
    Object(ShaderProgram& program, bool is = false, GLuint tex = 0, float x = 0, float y = 0, float width = 1, float height = 1, float velocity_x = 0, float velocity_y = 0);
    
    void text(const std::string& text, float size, float spacing, float x, float y);
    void display();
    void update();
    
private:
    ShaderProgram* program;
    
    bool istexture = false;
    GLuint texture;
    
    std::vector<float> vertices = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
    std::vector<float> texCoords = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
    
};

class Player: public Object {
public:
    using Object::Object;
};

class Enemy: public Object {
public:
    Enemy(ShaderProgram& program, bool is = false, GLuint tex = 0, float x = 0, float y = 0, float width = 1, float height = 1, float velocity_x = 0, float velocity_y = 0, bool up = true);
    void update(float elapsed);
private:
    bool up;
};

#endif /* Object_h */
