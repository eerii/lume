//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "imgui.h"
#include "imgui_sdl.h"

#include "renderer.h"

#include "s_texture.h"
#include "s_tilemap.h"
#include "s_collider.h"

#include "shader.h"

using namespace Verse;
using namespace Graphics;

#define W_TITLE "Project Verse"
#define W_WIDTH 1280
#define W_HEIGHT 720
#define W_FLAGS SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE

void Graphics::init() {
    
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    //Can't use modern opengl with sdl renderer, might change to custom renderer in the future
    
    //CREATE A WINDOW
    window = SDL_CreateWindow(W_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W_WIDTH, W_HEIGHT, W_FLAGS);
    if (window == nullptr) {
        log::error("Failed to create a Window", SDL_GetError());
    }
    #if _WIN32
    {
        int display = SDL_GetWindowDisplayIndex(window);
        float ddpi, hdpi, vdpi;
        if (SDL_GetDisplayDPI(display, &ddpi, &hdpi, &vdpi) == 0)
        {
            float hidpiRes = 96;
            float dpi = (ddpi / hidpiRes);
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
    SDL_SetWindowMinimumSize(window, 256, 256);
    
    //USE OPENGL
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    
    //REFRESH RATE
    Graphics::calculateRefreshRate();
    //log::info("Refresh Rate: %d", Graphics::refreshRate);
    
    //CREATE THE RENDERER
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    if (renderer == nullptr) {
        log::error("Failed to create a Renderer", SDL_GetError());
    }
    
    //COMPILE THE SHADERS
    SDL_RendererInfo rendererInfo;
        SDL_GetRendererInfo(renderer, &rendererInfo);

        if(!strncmp(rendererInfo.name, "opengl", 6)) {
            log::info("Using OpenGL");
    #ifndef __APPLE__
            if (!initGLExtensions()) {
                log::error("Couldn't init GL extensions!");
                SDL_Quit();
                exit(-1);
            }
    #endif
            programId = Graphics::Shader::compileProgram("res/shaders/palette.vertex", "res/shaders/palette.frag");
            log::info("Program ID: %d", programId);
        }
    
    //RENDER TARGET
    render_target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, W_WIDTH, W_HEIGHT); //TODO: Change to world width?
    
    //IMGUI
    ImGui::CreateContext();
    ImGuiSDL::Initialize(renderer, 1280, 720);
    
    //PALETTE
    palette_tex = loadTexture("res/graphics/palette_multi.png");
}

void Graphics::clear(Config &c) {
    if (c.use_shaders)
        SDL_SetRenderTarget(renderer, render_target);
    SDL_RenderClear(renderer);
}

void Graphics::render(Scene &scene, Config &c) {
    System::Tilemap::render(scene, renderer);
    System::Texture::render(scene, renderer);
    
    if (c.render_collision_boxes)
        System::Collider::render(scene, renderer);
    
    //Color the window
    SDL_SetRenderDrawColor(renderer, 133, 144, 188, 255);
}

void Graphics::display(Config &c) {
    if (c.use_shaders)
        presentWithShaders(c);
    else
        SDL_RenderPresent(renderer);
}

void Graphics::calculateRefreshRate() {
    int displayIndex = SDL_GetWindowDisplayIndex(window);
    
    SDL_DisplayMode mode;
    
    SDL_GetDisplayMode(displayIndex, 0, &mode);
    
    refreshRate = mode.refresh_rate;
}

SDL_Texture* Graphics::loadTexture(std::string path) {
    if (renderer == nullptr) {
        log::error("Renderer doesn't exist and you tried to load a texture");
        return nullptr;
    }
    
    //SDL_Surface* surface = IMG_Load(path.c_str());
    
    SDL_Texture* tex = IMG_LoadTexture(renderer, path.c_str());
    if (tex == nullptr) {
        log::error("There was an error loading the texture: %s", path.c_str());
    }
    return tex;
}

void Graphics::presentWithShaders(Config &c) {
    int oldProgramId = 0;
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderClear(renderer);
    
    if(programId != 0) {
        glGetIntegerv(GL_CURRENT_PROGRAM, &oldProgramId);
        glUseProgram(programId);
    }
    
    //GENERATE AND BIND TEXTURES
    glGenTextures(2, textures);
    
    glActiveTexture(GL_TEXTURE0);
    SDL_GL_BindTexture(render_target, NULL, NULL);
    glUniform1i(glGetUniformLocation(programId, "tex"), 0);
    
    glActiveTexture(GL_TEXTURE1);
    SDL_GL_BindTexture(palette_tex, NULL, NULL);
    glUniform1i(glGetUniformLocation(programId, "palette"), 1);
    
    glUniform1f(glGetUniformLocation(programId, "palette_index"), ((float)c.palette_index / (float)c.num_palettes) + 0.001);

    //COORDINATES FOR DRAWING
    float minx, miny, maxx, maxy;
    float minu, maxu, minv, maxv;

    minx = 0.0f;
    miny = 0.0f;
    maxx = W_WIDTH;
    maxy = W_HEIGHT;

    minu = 0.0f;
    maxu = 1.0f;
    minv = 1.0f;
    maxv = 0.0f;
    
    glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(minu, minv);
        glVertex2f(minx, miny);
        glTexCoord2f(maxu, minv);
        glVertex2f(maxx, miny);
        glTexCoord2f(minu, maxv);
        glVertex2f(minx, maxy);
        glTexCoord2f(maxu, maxv);
        glVertex2f(maxx, maxy);
    glEnd();
    
    SDL_GL_SwapWindow(window);

    if(programId != 0) {
        //TODO: Check this
        glUseProgram(oldProgramId);
    }
}
