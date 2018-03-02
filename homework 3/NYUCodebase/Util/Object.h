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
    Object(ShaderProgram& program, GLuint texture = 0, glm::vec3 pos = glm::vec3(0, 0, 0), glm::vec3 velo = glm::vec3(0, 0, 0));

    void update(float elapsed);
    void display();
    
    void scale(float size);
    
    void setupData(const XMLData& data);

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

    // vertices
    glm::vec3 shape;
    std::vector<float> vertices = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
    std::vector<float> texCoords = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};

};




#endif /* Object_h */
