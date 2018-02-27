// Yuxi Luo (yl4217), February 15, 2018
// Homework 2, PONG!, CS3113 Game Programming

#ifndef setUp_h
#define setUp_h

#include "ShaderProgram.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <string>

#include "Util/Object.h"

// from lecture slide Jan 31, 2018
GLuint LoadTexture(const char *filePath);

// untextured shader
ShaderProgram setUntextured();

// textured shader
ShaderProgram setTextured(const std::string& filepath, GLuint& texture);


// initialize the window
SDL_Window* setUp(const std::string& name);

// check keyboard event
void checkKeyboard(const SDL_Event& event, bool& done, bool& restart, bool& regame, Object& player);

#endif /* setUp_h */
