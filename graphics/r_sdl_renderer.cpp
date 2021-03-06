//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "r_sdl_renderer.h"
#include "r_present.h"
#include "r_shader.h"
#include "r_opengl.h"
#include "r_textures.h"

using namespace Verse;

#ifndef USE_OPENGL
namespace {
    SDL_Renderer* renderer;
    SDL_Texture* render_target;
    Vec2 previous_window_size;

    ui8 pid;
}
#endif

SDL_Renderer* Graphics::Renderer::SDL::getRenderer() {
    #ifdef USE_OPENGL
    return nullptr;
    #else
    return renderer;
    #endif
}

#ifndef USE_OPENGL
void Graphics::Renderer::SDL::create(Config &c, SDL_Window* window, ui8 &pid) {
    //Set OpenGL and create Renderer
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
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
        pid = Graphics::Shader::compileProgram("res/shaders/sdl.vertex", "res/shaders/sdl.frag");
        log::graphics("Program ID: %d", pid);
    }
    
    //Render Target
    render_target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, c.window_size.x, c.window_size.y);
    Graphics::linkRendererToTexture(renderer);
}

void Graphics::Renderer::SDL::clear(Config &c) {
    if (c.use_shaders) {
        SDL_SetRenderTarget(renderer, render_target);
        
        if (previous_window_size.x != c.window_size.x or previous_window_size.y != c.window_size.y) {
            previous_window_size = c.window_size;
            render_target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                              c.window_size.x, c.window_size.y);
        }
    }
    SDL_RenderClear(renderer);
}

void Graphics::Renderer::SDL::present(Config &c, SDL_Window* window, SDL_Texture* palette_tex, ui8 &pid) {
    if (c.use_shaders)
        Graphics::present(c, renderer, window, render_target, palette_tex, pid);
    else
        SDL_RenderPresent(renderer);
}

void Graphics::Renderer::SDL::destroy(SDL_Texture *palette_tex) {
    SDL_DestroyTexture(palette_tex);
    SDL_DestroyTexture(render_target);
    
    SDL_DestroyRenderer(renderer);
}

#endif
