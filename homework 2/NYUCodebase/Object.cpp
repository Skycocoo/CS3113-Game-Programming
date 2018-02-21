// Yuxi Luo (yl4217), February 15, 2018
// Homework 2, PONG!, CS3113 Game Programming

#include "Object.h"
using namespace std;


extern float screenWidth;
extern float screenHeight;

Object::Object(ShaderProgram& program, bool is, GLuint tex):program(&program), istexture(is), texture(tex){
    projectionMatrix.SetOrthoProjection(-screenWidth, screenWidth, -screenHeight, screenHeight, -1.0f, 1.0f);
}

void Object::text(const string& text, float size, float spacing, float x, float y){
    program->SetModelMatrix(modelMatrix);
    program->SetProjectionMatrix(projectionMatrix);
    program->SetViewMatrix(viewMatrix);
    
    float space = 5;
    
    float texture_size = 1.0/16.0f;
    vector<float> vertexData;
    vector<float> texCoordData;
    for (int i = 0; i < text.size(); i++) {
        int spriteIndex = (int)text[i];
        float texture_x = (float)(spriteIndex % 16) / 16.0f;
        float texture_y = (float)(spriteIndex / 16) / 16.0f;
        vertexData.insert(vertexData.end(), {
            ((size+spacing) * i)/space + (-0.5f * size) + x, 0.5f * size + y,
            ((size+spacing) * i)/space + (-0.5f * size) + x, -0.5f * size + y,
            ((size+spacing) * i)/space + (0.5f * size) + x, 0.5f * size + y,
            ((size+spacing) * i)/space + (0.5f * size) + x, -0.5f * size + y,
            ((size+spacing) * i)/space + (0.5f * size) + x, 0.5f * size + y,
            ((size+spacing) * i)/space + (-0.5f * size) + x, -0.5f * size + y,
        });
        texCoordData.insert(texCoordData.end(), {
            texture_x, texture_y,
            texture_x, texture_y + texture_size,
            texture_x + texture_size, texture_y,
            texture_x + texture_size, texture_y + texture_size,
            texture_x + texture_size, texture_y,
            texture_x, texture_y + texture_size,
        }); }
    
//    glUseProgram(program->programID);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, text.size() * 6);
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Object::display(){
    program->SetModelMatrix(modelMatrix);
    program->SetProjectionMatrix(projectionMatrix);
    program->SetViewMatrix(viewMatrix);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);
    
    if (istexture){
        glBindTexture(GL_TEXTURE_2D, texture);
        glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
        glEnableVertexAttribArray(program->texCoordAttribute);
    }
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program->positionAttribute);
    
    if (istexture) glDisableVertexAttribArray(program->texCoordAttribute);
}

void Object::scale(){
    modelMatrix.Scale(width, height, 1);

}

