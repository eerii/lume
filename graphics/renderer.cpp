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
            // If you want to use GLEW or some other GL extension handler, do it here!
            if (!initGLExtensions()) {
                std::cout << "Couldn't init GL extensions!" << std::endl;
                SDL_Quit();
                exit(-1);
            }
    #endif
            // Compile the shader
            programId = Graphics::Shader::compileProgram("res/shaders/std.vertex", "res/shaders/crt.fragment");
            log::info("Program ID: %d", programId);
        }
    
    //RENDER TARGET
    render_target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, W_WIDTH, W_HEIGHT); //TODO: Change to world width?
    
    //IMGUI
    ImGui::CreateContext();
    ImGuiSDL::Initialize(renderer, 1280, 720);
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
        presentBackBuffer(renderer, window, render_target, programId);
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

void Graphics::presentBackBuffer(SDL_Renderer *renderer, SDL_Window* win, SDL_Texture* backBuffer, ui32 pid) {
    GLuint programId = pid;
    GLint oldProgramId = 0;
    //Obtain textureid (in driverdata->texture)
    //Detach the texture
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderClear(renderer);

    SDL_GL_BindTexture(backBuffer, NULL, NULL);
    if(programId != 0) {
        glGetIntegerv(GL_CURRENT_PROGRAM, &oldProgramId);
        glUseProgram(programId);
    }

    GLfloat minx, miny, maxx, maxy;
    GLfloat minu, maxu, minv, maxv;

    // Coordenadas de la ventana donde pintar.
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
    SDL_GL_SwapWindow(win);

    if(programId != 0) {
        //TODO: Check this
        glUseProgram(oldProgramId);
    }
}
