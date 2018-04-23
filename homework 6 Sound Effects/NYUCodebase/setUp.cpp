// Yuxi Luo (yl4217), February 26, 2018
// CS3113 Game Programming


#include "setUp.hpp"
#include "stb_image.h"
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"

extern float screenRatio;
extern float screenHeight;
extern float screenWidth;
extern float splitScale;


using namespace std;

// from lecture slide Jan 31, 2018
GLuint LoadTexture(const char *filePath) {
    int w, h, comp;
    unsigned char* image = stbi_load(filePath, &w, &h, &comp, STBI_rgb_alpha);

    if(image == NULL) {
        cerr << "Unable to load image in the path " << *filePath << ". Make sure the path is correct\n";
        exit(1);
    }

    GLuint retTexture;
    glGenTextures(1, &retTexture);
    glBindTexture(GL_TEXTURE_2D, retTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(image);
    return retTexture;
}

// untextured shader
ShaderProgram setUntextured(){
    ShaderProgram program;
    program.Load(RESOURCE_FOLDER"Shad/vertex.glsl", RESOURCE_FOLDER"Shad/fragment.glsl");
    glUseProgram(program.programID);

    return program;
}

// textured shader
ShaderProgram setTextured(const string& filepath, GLuint& texture){
    ShaderProgram program;
    program.Load(RESOURCE_FOLDER"Shad/vertex_textured.glsl", RESOURCE_FOLDER"Shad/fragment_textured.glsl");
    texture = LoadTexture((RESOURCE_FOLDER + filepath).c_str());

    return program;
}


// initialize the window
SDL_Window* setUp(const string& name){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* displayWindow = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1020, 720, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

    glViewport(0, 0, 1020, 720);
    glClearColor(0.376, 0.509, 0.806, 0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // screen view point
    screenRatio = float(1020) / float(720);
    screenHeight = 5.0;
    screenWidth = screenHeight * screenRatio;
    
    // music
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 );
    Mix_Music* music = Mix_LoadMUS("Asset/my_music.mp3");
    if(!music) {
        cout << "Mix_LoadMUS(\"music.mp3\"): ";
        Mix_GetError();
        cout << endl;
        exit(1);
    }
    
    Mix_PlayMusic(music, -1);

    return displayWindow;
}

// check keyboard event
void checkKeyboard(const SDL_Event& event, bool& done){
    switch (event.type){
        case SDL_QUIT:
            done = true;
            break;
        case SDL_WINDOWEVENT_CLOSE:
            done = true;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.scancode == SDL_SCANCODE_Q) done = true;
            break;
    }
}
