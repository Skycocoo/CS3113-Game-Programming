// Yuxi Luo (yl4217), February 26, 2018
// Homework 3, Space Invaders, CS3113 Game Programming

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <cstdlib>
#include <vector>

#include "Util/Text.h"
#include "Util/Object.h"
#include "Util/XMLLoad.h"
#include "setUp.h"

using namespace std;
float screenRatio;
float screenHeight;
float screenWidth;
float splitScale;


// display the score for each side in the middle of the game
void displayGame(Text& disp){
    disp.display("a", 0.8, 1, 0, 0);
}

class SheetSprite {
public:
    SheetSprite(){};
    SheetSprite(GLuint textureID, float u, float v, float width, float height, float size): textureID(textureID), u(u), v(v), width(width), height(height), size(size){
//        cout << u << " " << v << " " << width << " " << height << " " << size << endl;
    };
    
    void Draw(ShaderProgram *program){
        GLfloat texCoords[] = {
            u, v+height,
            u+width, v,
            u, v,
            u+width, v,
            u, v+height,
            u+width, v+height
        };
        float aspect = width / height;
        float vertices[] = {
            -0.5f * size * aspect, -0.5f * size,
            0.5f * size * aspect, 0.5f * size,
            -0.5f * size * aspect, 0.5f * size,
            0.5f * size * aspect, 0.5f * size,
            -0.5f * size * aspect, -0.5f * size ,
            0.5f * size * aspect, -0.5f * size};
        
        program->SetModelMatrix(Matrix());
        program->SetProjectionMatrix(Matrix());
        program->SetViewMatrix(Matrix());
        
        glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
        glEnableVertexAttribArray(program->positionAttribute);
        
        
        glBindTexture(GL_TEXTURE_2D, textureID);
        glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
        glEnableVertexAttribArray(program->texCoordAttribute);
    
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(program->positionAttribute);
        glDisableVertexAttribArray(program->texCoordAttribute);
        
    }
    float size;
    GLuint textureID;
    float u;
    float v;
    float width;
    float height;
};

void Render(SheetSprite& enemySprite, ShaderProgram* program) {
    enemySprite.Draw(program);
}



int main(){
    // initial set up
    srand(time(NULL));
    SDL_Window* displayWindow = setUp("Homework 3 Space Invaders");
    
    
    XMLLoad xml ("Asset/sheet.xml");
    Data a = xml.getData("playerShip2_orange.png");
    GLuint texture;
    ShaderProgram shad = setTextured("Asset/sheet.png", texture);
    // assume the size of texture sheet is 1024 * 1024
    SheetSprite t(texture, a.x / 1024.0, a.y/ 1024.0, a.width/ 1024.0, a.height/ 1024.0, 0.2);
    

    GLuint texture2;
    ShaderProgram shad2 = setTextured("Asset/font1.png", texture2);
    Text disp(shad2, texture2);

    SDL_Event event;
    bool done = false;
    while (!done) {
        // keyboard event
        while (SDL_PollEvent(&event)) checkKeyboard(event, done);

        // display
        glClear(GL_COLOR_BUFFER_BIT);

        Render(t, &shad);
        displayGame(disp);
        
        SDL_GL_SwapWindow(displayWindow);
    }

    SDL_Quit();
    
    return 0;
}
