//
//  Object.cpp
//  NYUCodebase
//
//  Created by Yuxi Luo (Github @ Skycocoo) on 17/02/2018.
//  Copyright © 2018 Ivan Safrin. All rights reserved.
//

#include "Object.h"
#include "ShaderProgram.h"

extern float screenWidth;
extern float screenHeight;

Object::Object(ShaderProgram& program, bool is, GLuint tex):program(&program), istexture(is), texture(tex){
    projectionMatrix.SetOrthoProjection(-screenWidth, screenWidth, -screenHeight, screenHeight, -1.0f, 1.0f);
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

