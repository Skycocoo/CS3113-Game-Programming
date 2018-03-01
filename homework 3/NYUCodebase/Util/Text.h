// Yuxi Luo (yl4217), March 1, 2018
// CS3113 Game Programming

#ifndef Text_h
#define Text_h

#include "Object.h"

class Text: public Object{
public:
    Text(ShaderProgram& program, GLuint texture);
    
    void display(const std::string& text, float size, float spacing, float x, float y);
};

#endif /* Text_h */
