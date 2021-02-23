//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "imgui.h"
#include "imgui_sdl.h"

#include "renderer.h"

#include "s_texture.h"
#include "s_tilemap.h"
#include "s_collider.h"

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
    
    //REFRESH RATE
    Graphics::calculateRefreshRate();
    //log::info("Refresh Rate: %d", Graphics::refreshRate);
    
    //CREATE THE RENDERER
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        log::error("Failed to create a Renderer", SDL_GetError());
    }
    
    ImGui::CreateContext();
    ImGuiSDL::Initialize(renderer, 1280, 720);
}

void Graphics::clear() {
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

void Graphics::display() {
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
    
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (surface == nullptr) {
        log::error("There was an error loading the surface: %s", path.c_str());
        return nullptr;
    }
    
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
    if (tex == nullptr) {
        log::error("There was an error loading the texture: %s", path.c_str());
    }
    return tex;
}
