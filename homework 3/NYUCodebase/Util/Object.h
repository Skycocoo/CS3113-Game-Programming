// Yuxi Luo (yl4217), February 26, 2018
// CS3113 Game Programming

#ifndef Object_h
#define Object_h

#include <vector>
#include <string>

#include "ShaderProgram.h"
#include "../Math/Matrix.h"

#include "glm/glm.hpp"

// create an object class to handle parameters
class Object{
public:
    Object(ShaderProgram& program, bool istexture = false, GLuint texture = 0, glm::vec3 pos = glm::vec3(), glm::vec3 size = glm::vec3(), glm::vec3 velo = glm::vec3());

//    void text(const std::string& text, float size, float spacing, float x, float y);
    void display();
    void update(float elapsed);

protected:
    // shader
    ShaderProgram* program;
    
    // texture
    bool istexture = false;
    GLuint texture;
    
    // matrices
    Matrix projectionMatrix;
    Matrix modelMatrix;
    Matrix viewMatrix;

    // vectors
    glm::vec3 pos;
    glm::vec3 size;
    glm::vec3 velo;
    glm::vec3 acce;

    // vertices
    std::vector<float> vertices = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
    std::vector<float> texCoords = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};

};




#endif /* Object_h */
