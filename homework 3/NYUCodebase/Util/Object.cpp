// Yuxi Luo (yl4217), February 26, 2018
// CS3113 Game Programming

#include "Object.h"
using namespace std;

extern float screenWidth;
extern float screenHeight;
extern float splitScale;

Object::Object(ShaderProgram& program, GLuint texture, glm::vec3 pos, glm::vec3 size, glm::vec3 velo): program(&program), texture(texture), pos(pos), size(size), velo(velo){
    projectionMatrix.SetOrthoProjection(-screenWidth, screenWidth, -screenHeight, screenHeight, -1.0f, 1.0f);
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

void Object::update(float elapsed){
    modelMatrix.Identity();
    
    modelMatrix.Translate(velo.x, velo.y, velo.z);
    modelMatrix.Scale(size.x, size.y, size.z);
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






