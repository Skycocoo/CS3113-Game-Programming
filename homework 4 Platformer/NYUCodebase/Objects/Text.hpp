// Yuxi Luo (yl4217), March 1, 2018
// CS3113 Game Programming

#ifndef Text_hpp
#define Text_hpp

#include "Object.hpp"

class Text: public Object{
public:
    Text();
    Text(ShaderProgram* program, GLuint texture);

    void render(const std::string& text, float size, float spacing, float x, float y);
};

#endif /* Text_hpp */
