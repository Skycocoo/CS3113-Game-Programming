// Yuxi Luo (yl4217), February 26, 2018
// CS3113 Game Programming

#ifndef Object_h
#define Object_h

#include <vector>
#include <string>

#include "../Util/XMLLoad.h"
#include "../Util/ShaderProgram.h"
#include "../Math/Matrix.h"

#include "glm/glm.hpp"

// create an object class to handle parameters
class Object{
public:
    Object();
    Object(ShaderProgram* program, GLuint texture = 0, const glm::vec3& pos = glm::vec3(0, 0, 0));

    virtual void update(float elapsed = 0);
    virtual void render();
    virtual bool collide (const Object& rhs);

    void setScale(float size);
    void setShape(const glm::vec3& shape);
    void setRotate(float rot); // rotate: angle (in radian)
    void setPos(const glm::vec3& pos);

    void setData(const XMLData& data);

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


    float rotate = 0.0;

    // vertices
    glm::vec3 shape; // shape.x: width; shape.y: height; shape.z: 0 / 1
    std::vector<float> vertices = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
    std::vector<float> texCoords = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
};






#endif /* Object_h */
