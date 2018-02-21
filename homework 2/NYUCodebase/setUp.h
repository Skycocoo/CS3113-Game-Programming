//
//  setUp.h
//  NYUCodebase
//
//  Created by 罗予希 on 20/02/2018.
//  Copyright © 2018 Ivan Safrin. All rights reserved.
//

#ifndef setUp_h
#define setUp_h

#include "ShaderProgram.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <string>

#include "Object.h"

// from lecture slide Jan 31, 2018
GLuint LoadTexture(const char *filePath);

// untextured shader
ShaderProgram setUntextured();

// textured shader
ShaderProgram setTextured(const std::string& filepath, GLuint& texture);


// initialize the window
SDL_Window* setUp(const std::string& name);

// check keyboard event
void checkKeyboard(const SDL_Event& event, bool& done, Object& player);

#endif /* setUp_h */
