//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "imgui.h"
#include "imgui_sdl.h"

#include "s_texture.h"
#include "s_tilemap.h"
#include "s_collider.h"
#include "s_light.h"

#include "r_pipeline.h"
#include "r_shader.h"
#include "r_opengl.h"
#include "r_present.h"
#include "r_textures.h"

using namespace Verse;
using namespace Graphics;

#define W_WIDTH 1024
#define W_HEIGHT 720
#define W_FLAGS SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE

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
    window = SDL_CreateWindow((c.name + " - Version " + c.version).c_str(),
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              W_WIDTH, W_HEIGHT, W_FLAGS);
    if (window == nullptr) {
        log::error("Failed to create a Window", SDL_GetError());
    }
    #if _WIN32
    {
        int display = SDL_GetWindowDisplayIndex(window);
        float ddpi, hdpi, vdpi;
        if (SDL_GetDisplayDPI(display, &ddpi, &hdpi, &vdpi) == 0)
        {
            float hidpi_res = 96;
            float dpi = (ddpi / hidpi_res);
            if (dpi != 1)
            {
                SDL_DisplayMode mode;
                SDL_GetDesktopDisplayMode(display, &mode);
                SDL_SetWindowPosition(window, (int)(mode.w - config->width * dpi) / 2, (int)(mode.h - config->height * dpi) / 2);
                SDL_SetWindowSize(window, (int)(config->width * dpi), (int)(config->height * dpi));
            }
        }
    }
    #endif
    SDL_SetWindowResizable(window, SDL_TRUE);
    SDL_SetWindowMinimumSize(window, 256, 180);
    
    //USE OPENGL
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    
    //REFRESH RATE
    Graphics::calculateRefreshRate();
    //log::graphics("Refresh Rate: %d", Graphics::refreshRate);
    
    //CREATE THE RENDERER
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
    render_target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, W_WIDTH, W_HEIGHT); //TODO: Change to world width?
    
    //IMGUI
    ImGui::CreateContext();
    ImGuiSDL::Initialize(renderer, W_WIDTH, W_HEIGHT);
    
    //LINK TEXTURES TO RENDERER
    Graphics::linkRendererToTexture(renderer);
    
    //PALETTE
    palette_tex = loadTexture("res/graphics/palette_multi.png");
    
    //LIGHT
    System::Light::init(renderer, c);
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


void Graphics::render(Scene &scene, Config &c) {
    System::Tilemap::render(scene, renderer, c);
    System::Texture::render(scene, renderer, c);
    System::Light::render(scene, c);
    
    if (c.render_collision_boxes)
        System::Collider::render(scene, renderer, c);
    
    SDL_SetRenderDrawColor(renderer, c.background_color[0], c.background_color[1], c.background_color[2], c.background_color[3]);
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
