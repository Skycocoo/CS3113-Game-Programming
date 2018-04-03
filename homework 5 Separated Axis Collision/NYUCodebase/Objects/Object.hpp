// Yuxi Luo (yl4217), February 26, 2018
// CS3113 Game Programming

#ifndef Object_hpp
#define Object_hpp

#include <vector>
#include <string>

#include "../Util/XMLLoad.hpp"
#include "../Util/ShaderProgram.hpp"
#include "../Math/Matrix.hpp"

#include "glm/glm.hpp"


// collision flags for four sides
struct Coll{
    bool top = false, bottom = false, left = false, right = false;
    void reset(){
        top = false;
        bottom = false;
        left = false;
        right = false;
    }
};

// create an object class to handle parameters
class Object{
    friend class Tile;
public:
    Coll coll;

    Object();
    Object(ShaderProgram* program, GLuint texture = 0, const glm::vec3& pos = glm::vec3(0, 0, 0));

    virtual void update(float elapsed = 0);
    virtual void render(const Matrix& view = Matrix());
    virtual bool collide(const Object& rhs);

    bool satCollide(Object& rhs);

    void setScale(float size);
    void setShape(const glm::vec3& shape);
    void setRotate(float rot); // rotate: angle (in radian)
    void setPos(const glm::vec3& pos);
    void setPos(float x, float y);

    void setProject(float proj);

    void setData(const XMLData& data);

    const glm::vec3& getCenter() const {
        return pos;
    }

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
    float scale = 1.0;

    // vertices
    glm::vec3 shape; // shape.x: width; shape.y: height; shape.z: 0 / 1
    std::vector<float> vertices = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
    std::vector<float> texCoords = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};

};

bool TestSATSeparationForEdge(float edgeX, float edgeY, const std::vector<std::pair<float,float>> &points1, const std::vector<std::pair<float,float>> &points2, std::pair<float,float> &penetration);
bool PenetrationSort(const std::pair<float,float> &p1, const std::pair<float,float> &p2);
bool CheckSATCollision(const std::vector<std::pair<float,float>> &e1Points, const std::vector<std::pair<float,float>> &e2Points, std::pair<float,float> &penetration);


#endif /* Object_hpp */
