// Yuxi Luo (yl4217), February 26, 2018
// CS3113 Game Programming

#include "Object.h"
using namespace std;

extern float screenWidth;
extern float screenHeight;
extern float splitScale;

Object::Object(){}


Object::Object(ShaderProgram* program, GLuint texture, const glm::vec3& pos, const glm::vec3& velo): program(program), texture(texture), pos(pos), velo(velo), shape(glm::vec3(1, 1, 1)), fric(glm::vec3(0.01, 0.01, 0.01)), grav(glm::vec3(0, -0.1, 0)){
    projectionMatrix.SetOrthoProjection(-screenWidth, screenWidth, -screenHeight, screenHeight, -1.0f, 1.0f);
}


void Object::update(float elapsed){
    modelMatrix.Identity();

    modelMatrix.Translate(pos.x, pos.y, pos.z);
    modelMatrix.Scale(shape.x, shape.y, shape.z);

    modelMatrix.Rotate(rotate);
}


void Object::render(){
    program->SetModelMatrix(modelMatrix);
    program->SetProjectionMatrix(projectionMatrix);
    program->SetViewMatrix(viewMatrix);

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

void Object::setScale(float size){
    this->shape *= size;
}

void Object::setShape(const glm::vec3& shape){
    this->shape = shape;
}


void Object::setVelo(const glm::vec3& velo){
    this->velo = velo;
}

void Object::setVelo(float x, float y, float z){
    this->velo.x = x;
    this->velo.y = y;
    this->velo.z = z;
}

void Object::setRotate(float rot){
    this->rotate = rot;
}

void Object::setPos(const glm::vec3& pos){
    this->pos = pos;
}

void Object::setData(const XMLData& data){
    // assume the shape of sheetsprite is 1024 * 1024
    float u = data.x / 1024.0, v = data.y / 1024.0, width = data.width / 1024.0, height = data.height / 1024.0;

    // rescale the image so that the max edge length is 1
    float w = (width / height < 1) ? width / height : 1.0, h = (width / height < 1) ? 1.0 : height / width;

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


// collision detection
bool Object::collide(const Object& rhs){
    bool collide = false;
        
    float   objUp = pos.y + shape.y / 2,
    objDown = pos.y - shape.y / 2,
    objLeft = pos.x - shape.x / 2,
    objRight = pos.x + shape.x / 2;
    
    
    float enUp = rhs.pos.y + rhs.shape.y / 2,
    enDown = rhs.pos.y - rhs.shape.y / 2,
    enLeft = rhs.pos.x - rhs.shape.x / 2,
    enRight = rhs.pos.x + rhs.shape.x / 2;
    
    
    // intersecting
    if (!(objUp < enDown || objLeft > enRight || objDown > enUp || objRight < enLeft)) collide = true;
    
    // avoid stuck in the collision bar in collision handling function
    
    return collide;
}




//Enemy::Enemy(ShaderProgram& program, bool is, GLuint tex, float x, float y, float width, float height, float velocity_x, float velocity_y, bool up): Object(program, is, tex, x, y, width, height, velocity_x, velocity_y), up(up){}
//
//void Enemy::update(float elapsed) {
//    // update the slide bar to be automatically moved by time
//    float distance = elapsed * velocity_y;
//
//    if (y > screenHeight - distance - height / 2 - splitScale / 2) up = false;
//    else if (y < -screenHeight + distance + height / 2 + splitScale / 2) up = true;
//    if (up) y += distance;
//    else y -= distance;
//
//    Object::update();
//}
