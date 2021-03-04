//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "r_sdl_renderer.h"
#include "r_shader.h"
#include "r_opengl.h"

using namespace Verse;

SDL_Renderer* Graphics::Renderer::createSDLRenderer(Config &c, SDL_Window* window, SDL_Texture* render_target, ui8 &pid) {
    //Set OpenGL and create Renderer
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    if (renderer == nullptr) {
        log::error("Failed to create a Renderer", SDL_GetError());
    }
    
    SDL_RendererInfo rendererInfo;
    SDL_GetRendererInfo(renderer, &rendererInfo);
    if(!strncmp(rendererInfo.name, "opengl", 6)) {
        log::graphics("OpenGL Version: %s", glGetString(GL_VERSION));
        #ifndef __APPLE__
        //Initialize Extensions
        if (!initGLExtensions()) {
            log::error("Couldn't init GL extensions!");
            SDL_Quit();
            exit(-1);
        }
        #endif
        //Compile Shaders
        pid = Graphics::Shader::compileProgram("res/shaders/palette.vertex", "res/shaders/palette.frag");
        log::graphics("Program ID: %d", pid);
    }
    
    //Render Target
    render_target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, c.window_size.x, c.window_size.y);
    
    return renderer;
}
