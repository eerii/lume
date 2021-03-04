//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "imgui.h"
#include "imgui_sdl.h"
#include "gui.h"

#include "s_texture.h"
#include "s_tilemap.h"
#include "s_collider.h"
#include "s_light.h"

#include "r_pipeline.h"
#include "r_window.h"
#include "r_shader.h"
#include "r_opengl.h"
#include "r_present.h"
#include "r_textures.h"

using namespace Verse;
using namespace Graphics;

namespace {
    SDL_Window *window;
    SDL_Renderer *renderer;

    int refresh_rate = 60;

    SDL_Texture *render_target;
    SDL_Texture *palette_tex;
    Vec2 previous_window_size;

    ui8 pid;
}

void Graphics::init(Config &c) {
    
    //Can't use modern opengl with sdl renderer, might change to custom renderer in the future
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    //CREATE A WINDOW
    window = Graphics::Window::createWindow(c);
    
    //REFRESH RATE
    Graphics::calculateRefreshRate();
    log::graphics("Refresh Rate: %d", refresh_rate);
    
    //CREATE THE RENDERER TODO: Deprecate
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    if (renderer == nullptr) {
        log::error("Failed to create a Renderer", SDL_GetError());
    }
    
    //COMPILE THE SHADERS
    SDL_RendererInfo rendererInfo;
        SDL_GetRendererInfo(renderer, &rendererInfo);
        if(!strncmp(rendererInfo.name, "opengl", 6)) {
            log::graphics("OpenGL Version: %s", glGetString(GL_VERSION));
    #ifndef __APPLE__
            if (!initGLExtensions()) {
                log::error("Couldn't init GL extensions!");
                SDL_Quit();
                exit(-1);
            }
    #endif
            pid = Graphics::Shader::compileProgram("res/shaders/palette.vertex", "res/shaders/palette.frag");
            log::graphics("Program ID: %d", pid);
        }
    
    //RENDER TARGET
    render_target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, c.window_size.x, c.window_size.y); //TODO: Change to world width?
    
    //IMGUI
    ImGui::CreateContext();
    ImGuiSDL::Initialize(renderer, c.window_size.x, c.window_size.y);
    
    //LINK TEXTURES TO RENDERER
    Graphics::linkRendererToTexture(renderer);
    
    //PALETTE
    palette_tex = loadTexture("res/graphics/palette_multi.png");
    
    //LIGHT
    System::Light::init(renderer, c);
}


void Graphics::render(Scene &scene, Config &c, ui16 fps) {
    //PRERENDER GUI
    prerender(scene, c, fps);
    
    //CLEAR
    clear(c);
    
    //RENDER TEXTURES
    System::Tilemap::render(scene, renderer, c);
    System::Texture::render(scene, renderer, c);
    System::Light::render(scene, c);
    
    //RENDER DEBUG
    if (c.render_collision_boxes)
        System::Collider::render(scene, renderer, c);
    
    //BACKGROUND
    SDL_SetRenderDrawColor(renderer, c.background_color[0], c.background_color[1], c.background_color[2], c.background_color[3]);
    
    //RENDER GUI
    if (c.enable_gui)
        Gui::render();
    
    //DISPLAY
    display(c);
}


void Graphics::prerender(Scene &scene, Config &c, ui16 fps) {
    if (c.enable_gui)
        Gui::prerender(scene, c, fps);
}


void Graphics::clear(Config &c) {
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


void Graphics::display(Config &c) {
    if (c.use_shaders)
        Graphics::present(c, renderer, window, render_target, palette_tex, pid);
    else
        SDL_RenderPresent(renderer);
}


void Graphics::destroy() {
    SDL_DestroyTexture(palette_tex);
    SDL_DestroyTexture(render_target);
    
    SDL_DestroyRenderer(renderer);
    
    SDL_DestroyWindow(window);
}


void Graphics::calculateRefreshRate() {
    int displayIndex = SDL_GetWindowDisplayIndex(window);
    
    SDL_DisplayMode mode;
    SDL_GetDisplayMode(displayIndex, 0, &mode);
    
    refresh_rate = mode.refresh_rate;
}


int Graphics::getRefreshRate() {
    return refresh_rate;
}
