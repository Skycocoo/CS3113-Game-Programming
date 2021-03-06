// Yuxi Luo (yl4217), February 26, 2018
// CS3113 Game Programming

#include "Object.hpp"
#include "../Util/SATCollision.hpp"

#include <math.h>
#include <utility>

using namespace std;

extern float screenWidth;
extern float screenHeight;
extern float splitScale;

// constructors
Object::Object(){}
Object::Object(ShaderProgram* program, GLuint texture, const glm::vec3& pos): program(program), texture(texture), pos(pos), shape(1, 1, 1){
    projectionMatrix.SetOrthoProjection(-screenWidth, screenWidth, -screenHeight, screenHeight, -1.0f, 1.0f);
    satPoints();
}

// update & render
void Object::update(float elapsed){
    modelMatrix.Identity();

    modelMatrix.Translate(pos.x, pos.y, pos.z);
    modelMatrix.Rotate(rotate);
    modelMatrix.Scale(scale, scale, scale);
}

void Object::render(const Matrix& view){
    // viewMatrix = view;
    glUseProgram(program->programID);

    program->SetModelMatrix(modelMatrix);
    program->SetProjectionMatrix(projectionMatrix);
    program->SetViewMatrix(view);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);

    if (glIsTexture(texture)){
        glBindTexture(GL_TEXTURE_2D, texture);
        glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
        glEnableVertexAttribArray(program->texCoordAttribute);
    }
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program->positionAttribute);

    if (glIsTexture(texture)) glDisableVertexAttribArray(program->texCoordAttribute);
}

// collision detection
// separate axis (x and y) for collision handling
bool Object::collide(const Object& rhs) {
    coll.reset();
    bool collide = false;

    float objUp = pos.y + shape.y / 2,
    objDown = pos.y - shape.y / 2,
    objLeft = pos.x - shape.x / 2,
    objRight = pos.x + shape.x / 2;

    float enUp = rhs.pos.y + rhs.shape.y / 2,
    enDown = rhs.pos.y - rhs.shape.y / 2,
    enLeft = rhs.pos.x - rhs.shape.x / 2,
    enRight = rhs.pos.x + rhs.shape.x / 2;


    // intersecting
    if (!(objUp < enDown || objLeft > enRight || objDown > enUp || objRight < enLeft)) collide = true;
    // if (collide) std::cout << objUp << " " << enDown << " " << (objUp > enDown)<< std::endl;

    if (collide){
        if (objUp >= enDown && pos.y < rhs.pos.y) coll.top = true;
        if (objLeft <= enRight && pos.x > rhs.pos.x) coll.left = true;
        if (objDown <= enUp && pos.y > rhs.pos.y) coll.bottom = true;
        if (objRight >= enLeft && pos.x < rhs.pos.x) coll.right = true;
    } else coll.reset();

    if (coll.left || coll.right){
        float pen = fabs(fabs(pos.x - rhs.pos.x) - shape.x / 2 - rhs.shape.x / 2);
        if (coll.left) pos.x += pen + 0.0001;
        else pos.x -= pen - 0.0001;
    } else if (coll.top || coll.bottom){
        float pen = fabs(fabs(pos.y - rhs.pos.y) - shape.y / 2 - rhs.shape.y / 2);
        if (coll.bottom) pos.y += pen + 0.0001;
        else pos.y -= pen - 0.0001;
    }

    return collide;
}


// SAT collision detection
void Object::satPoints(){
    points.clear();

    // shape is taken care of in modelMatrix (?)
    points.push_back(glm::vec3(-0.5 * shape.x, -0.5 * shape.y, 0));
    points.push_back(glm::vec3(0.5 * shape.x, -0.5 * shape.y, 0));
    points.push_back(glm::vec3(0.5 * shape.x, 0.5 * shape.y, 0));
    points.push_back(glm::vec3(-0.5 * shape.x, 0.5 * shape.y, 0));
}


bool Object::satCollide(const Object& rhs){
    coll.reset();

    std::pair<float,float> penetration;
    std::vector<std::pair<float,float>> e1Points;
    std::vector<std::pair<float,float>> e2Points;

    for (size_t i = 0; i < points.size(); i++){
        glm::vec3 point = modelMatrix * points[i];
        e1Points.push_back(std::make_pair(point.x, point.y));
    }

    for (size_t i = 0; i < rhs.points.size(); i++){
        glm::vec3 point = rhs.modelMatrix * rhs.points[i];
        e2Points.push_back(std::make_pair(point.x, point.y));
    }

    // std::cout << coll;

    if (CheckSATCollision(e1Points, e2Points, penetration)){
        // std::cout << penetration.first << " " << penetration.second << std::endl;
        if (penetration.first == 0 && penetration.second == 0) {
            return false;
        }

        if (penetration.first > 0) coll.left = true;
        if (penetration.first < 0) coll.right = true;
        if (penetration.second > 0) coll.bottom = true;
        if (penetration.second < 0) coll.top = true;

        // std::cout << coll;

        pos.x += penetration.first;
        pos.y += penetration.second;

        return true;
    }

    return false;
}



// setters

void Object::setShader(ShaderProgram* program){
    this->program = program;
}

void Object::setScale(float size){
    this->scale = size;
    this->shape *= size;
}

void Object::setShape(const glm::vec3& shape){
    this->scale = 1.0;

    float w = shape.x / shape.y;
    float h = 1.0;
    vertices = {
        -0.5f * w, -0.5f * h,
        0.5f * w, -0.5f * h,
        0.5f * w, 0.5f * h,
        -0.5f * w, -0.5f * h,
        0.5f * w, 0.5f * h,
        -0.5f * w, 0.5f * h ,
    };
    this->shape.x = w;
    this->shape.y = h;
}

void Object::setRotate(float rot){
    this->rotate = rot;
}

void Object::setPos(const glm::vec3& pos){
    this->pos = pos;
}

void Object::setPos(float x, float y){
    this->pos.x = x;
    this->pos.y = y;
}

void Object::setProject(float proj){
    projectionMatrix.SetOrthoProjection(-screenWidth * proj, screenWidth * proj, -screenHeight * proj, screenHeight * proj, -1.0f, 1.0f);
}

void Object::setData(const XMLData& data){
    // assume the shape of sheetsprite is 1024 * 512
    float u = data.x / 1024.0,
          v = data.y / 512.0,
          width = data.width / 1024.0,
          height = data.height / 512.0;

    // rescale the image so that the max edge length is 1
    float w = (data.width / data.height < 1) ? data.width / data.height : 1.0,
          h = (data.width / data.height < 1) ? 1.0 : data.height / data.width;

    vertices = {
      -0.5f * w, -0.5f * h,
      0.5f * w, -0.5f * h,
      0.5f * w, 0.5f * h,
      -0.5f * w, -0.5f * h,
      0.5f * w, 0.5f * h,
      -0.5f * w, 0.5f * h ,
    };


    texCoords = {
        u, v+height,
        u+width, v+height,
        u+width, v,
        u, v+height,
        u+width, v,
        u, v
    };

    shape.x = w;
    shape.y = h;
    satPoints();

    shape *= scale;
}

// getters

const glm::vec3& Object::getPos() const {
    return pos;
}

// linear interpolation

void Object::lerp(glm::vec3& orig, const glm::vec3& prop, const glm::vec3& tar) const {
    orig = (glm::vec3(1, 1, 1) - prop) * orig + prop * tar;
}
