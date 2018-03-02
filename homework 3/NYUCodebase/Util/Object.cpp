// Yuxi Luo (yl4217), February 26, 2018
// CS3113 Game Programming

#include "Object.h"
using namespace std;

extern float screenWidth;
extern float screenHeight;
extern float splitScale;

Object::Object(ShaderProgram& program, GLuint texture, glm::vec3 pos, glm::vec3 velo): program(&program), texture(texture), pos(pos), velo(velo), shape(glm::vec3(1, 1, 1)){
    projectionMatrix.SetOrthoProjection(-screenWidth, screenWidth, -screenHeight, screenHeight, -1.0f, 1.0f);
}


void Object::update(float elapsed){
    modelMatrix.Identity();
    
    modelMatrix.Translate(velo.x, velo.y, velo.z);
    modelMatrix.Scale(shape.x, shape.y, shape.z);
}


void Object::display(){
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

void Object::scale(float size){
    shape *= size;
}

void Object::setupData(const XMLData& data){
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
    std::cout << w << " " << h << std::endl;

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






