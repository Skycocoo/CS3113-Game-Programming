// Yuxi Luo (yl4217), March 27, 2018
// CS3113 Game Programming

#include "StaticObj.hpp"

extern ShaderProgram textured;

StaticObj::StaticObj(): Object::Object(){}

StaticObj::StaticObj(GLuint texture, const glm::vec3& pos): Object(&textured, texture, pos){}

// void StaticObj::update(float elapsed){
//     Object::update();
// }
//
// void StaticObj::render(){
//     Object::render();
// }
