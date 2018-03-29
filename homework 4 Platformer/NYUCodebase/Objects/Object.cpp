// Yuxi Luo (yl4217), February 26, 2018
// CS3113 Game Programming

#include "Object.hpp"
#include <math.h>

using namespace std;

extern float screenWidth;
extern float screenHeight;
extern float splitScale;

Object::Object(){}

Object::Object(ShaderProgram* program, GLuint texture, const glm::vec3& pos): program(program), texture(texture), pos(pos), shape(1, 1, 1){
    projectionMatrix.SetOrthoProjection(-screenWidth, screenWidth, -screenHeight, screenHeight, -1.0f, 1.0f);
}

void Object::update(float elapsed){
    modelMatrix.Identity();

    modelMatrix.Translate(pos.x, pos.y, pos.z);
    modelMatrix.Scale(scale, scale, scale);

    modelMatrix.Rotate(rotate);
}


void Object::render(const Matrix& view){
    // viewMatrix = view;

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
// separate axis for collision handling
bool Object::collide(const Object& rhs) {
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


void Object::setScale(float size){
    this->scale = size;
    this->shape *= size;
}

void Object::setShape(const glm::vec3& shape){
    this->shape = shape;
}

void Object::setRotate(float rot){
    this->rotate = rot;
}

void Object::setPos(const glm::vec3& pos){
    this->pos = pos;
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
        0.5f * w, 0.5f * h,
        -0.5f * w, 0.5f * h,
        0.5f * w, 0.5f * h,
        -0.5f * w, -0.5f * h ,
        0.5f * w, -0.5f * h
    };

    texCoords = {
        u, v+height,
        u+width, v,
        u, v,
        u+width, v,
        u, v+height,
        u+width, v+height
    };

    shape.x = w;
    shape.y = h;
}
