//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "r_renderer.h"
#include "r_opengl.h"
#include "r_shader.h"

using namespace Verse::Graphics;

void Verse::Graphics::Renderer::createRenderer(ui8 &pid) {
    //log::graphics("OpenGL Version: %s", glGetString(GL_VERSION));
    
    //Create Renderer
    
    
    //Initialize Extensions
    #ifndef __APPLE__
    if (!initGLExtensions()) { //TODO: Fix this
        log::error("Couldn't init GL extensions!");
        SDL_Quit();
        exit(-1);
    }
    #endif
    
    //Compile Shaders
    //pid = Graphics::Shader::compileProgram("res/shaders/palette.vertex", "res/shaders/palette.frag");
    //log::graphics("Program ID: %d", pid);
}
