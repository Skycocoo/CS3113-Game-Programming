//
//  Object.h
//  NYUCodebase
//
//  Created by Yuxi Luo (Github @ Skycocoo) on 17/02/2018.
//  Copyright © 2018 Ivan Safrin. All rights reserved.
//

#ifndef Object_h
#define Object_h

#include <vector>
#include <string>
#include "Matrix.h"
#include "ShaderProgram.h"

// create an object class to handle parameters
class Object{
public:
    // matrices
    Matrix projectionMatrix;
    Matrix modelMatrix;
    Matrix viewMatrix;
    
    // positions
    float x = 0;
    float y = 0;
    float width = 1;
    float height = 1;
    float velocity_x = 1;
    float velocity_y = 1;
    
    Object(ShaderProgram& program, bool is = false, GLuint tex = 0);
    
    void text(const std::string& text, float size, float spacing);
    void display();
    void scale();
    
private:
    ShaderProgram* program;
    
    bool istexture = false;
    GLuint texture;
    
    std::vector<float> vertices = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
    std::vector<float> texCoords = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
    
};

#endif /* Object_h */
