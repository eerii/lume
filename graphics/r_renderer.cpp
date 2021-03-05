//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "r_renderer.h"
#include "r_opengl.h"
#include "r_shader.h"

#ifdef USE_OPENGL

using namespace Verse;
using namespace Graphics;

namespace {
    SDL_GLContext context;
}

void Graphics::Renderer::GL::create(Config &c, SDL_Window* window, ui8 &pid) {
    //CONTEXT
    context = SDL_GL_CreateContext(window);
    if (context == NULL) {
        log::error("Error creating OpenGL Context: %s", SDL_GetError());
        SDL_Quit();
        exit(-1);
    }
    
    //EXTENSIONS
    #ifndef __APPLE__
    //Initialize Extensions
    if (!initGLExtensions()) {
        log::error("Couldn't init GL extensions!");
        SDL_Quit();
        exit(-1);
    }
    #endif
    
    //V-SYNC
    SDL_GL_SetSwapInterval(1);
    
    //LIBRARY
    if (SDL_GL_LoadLibrary(NULL) != 0) {
        log::error("Couldn't init GL Library!");
        SDL_Quit();
        exit(-1);
    }
    
    SDL_GL_UnloadLibrary();
    
    //INFORMATION
    log::graphics("---");
    log::graphics("Vendor:          %s", glGetString(GL_VENDOR));
    log::graphics("Renderer:        %s", glGetString(GL_RENDERER));
    log::graphics("OpenGL Version:  %s", glGetString(GL_VERSION));
    log::graphics("GLSL Version:    %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
    log::graphics("---");
    
    //SHADERS
    //TODO: Port shaders
    pid = Graphics::Shader::compileProgram("res/shaders/opengl.vertex", "res/shaders/opengl.frag");
    glUseProgram(pid);
    //Graphics::Shader::validateProgram(pid);
    log::graphics("Program ID: %d", pid);
    log::graphics("---");
}

void Graphics::Renderer::GL::clear() {
    
}

void Graphics::Renderer::GL::present(SDL_Window* window, ui8 &pid) {
    
}

void Graphics::Renderer::GL::destroy() {
    //OPENGL
    
    //SDL
    SDL_GL_DeleteContext(context);
}

#endif
