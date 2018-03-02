// Yuxi Luo (yl4217), February 26, 2018
// CS3113 Game Programming

#ifndef Object_h
#define Object_h

#include <vector>
#include <string>

#include "XMLData.h"
#include "ShaderProgram.h"
#include "../Math/Matrix.h"

#include "glm/glm.hpp"

// create an object class to handle parameters
class Object{
public:
    Object(ShaderProgram* program, GLuint texture = 0, glm::vec3 pos = glm::vec3(0, 0, 0), glm::vec3 velo = glm::vec3(0, 0, 0));

    void update(float elapsed = 0);
    void display();

    void setScale(float size);
    void setVelo(const glm::vec3& velo);
    void setVelo(float x, float y, float z = 1.0);
    void setRotate(float rot); // rotate: angle (in radian)
    void setData(const XMLData& data);
    
    bool collide (const Object& rhs);

protected:
    // shader
    ShaderProgram* program;

    // texture
    GLuint texture;

    // matrices
    Matrix projectionMatrix;
    Matrix modelMatrix;
    Matrix viewMatrix;

    // physics
    glm::vec3 pos;
    glm::vec3 velo;
    glm::vec3 acce;

    float rotate = 0.0;

    // vertices
    glm::vec3 shape; // shape.x: width; shape.y: height; shape.z: 0 / 1
    std::vector<float> vertices = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
    std::vector<float> texCoords = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};

};




#endif /* Object_h */
