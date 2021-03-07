//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "imgui.h"
#ifndef USE_OPENGL
#include "imgui_sdl.h"
#endif
#include "gui.h"

#include "s_texture.h"
#include "s_tilemap.h"
#include "s_collider.h"
#include "s_light.h"

#include "r_pipeline.h"
#include "r_window.h"
#include "r_shader.h"
#include "r_opengl.h"
#include "r_renderer.h"
#include "r_sdl_renderer.h"
#include "r_present.h"
#include "r_textures.h"

using namespace Verse;
using namespace Graphics;

namespace {
    SDL_Window *window;
    int refresh_rate = 60;

    Tex* palette_tex;
}

void Graphics::init(Config &c) {
    
    //MODERN OPENGL
    #ifdef USE_OPENGL
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    #ifndef __APPLE__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    #endif //!__APPLE__
    #endif
    
    //CREATE A WINDOW
    window = Graphics::Window::createWindow(c);
    
    //REFRESH RATE
    Graphics::calculateRefreshRate();
    log::graphics("Refresh Rate: %d", refresh_rate);
    
    //RENDERER
    #ifdef USE_OPENGL
    Renderer::GL::create(c, window);
    #else
    Renderer::SDL::create(c, window);
    #endif
    
    //IMGUI TODO: Moved into renderer create
    #ifndef USE_OPENGL
    ImGui::CreateContext();
    ImGuiSDL::Initialize(Renderer::SDL::getRenderer(), c.window_size.x, c.window_size.y);
    #endif
    
    //PALETTE
    ui32 palette_pointer = 0;
    palette_tex = loadTexture("res/graphics/palette_multi.png", palette_pointer);
    
    //LIGHT
    System::Light::init(Renderer::SDL::getRenderer(), c);
}


void Graphics::render(Scene &scene, Config &c, ui16 fps) {
    //CLEAR
#ifdef USE_OPENGL
    Renderer::GL::clear(c);
#else
    Renderer::SDL::clear(c);
#endif
    
    //PRERENDER GUI
    if (c.enable_gui)
        Gui::prerender(scene, c, fps, window);
    
    //RENDER TEXTURES
    System::Tilemap::render(scene, Renderer::SDL::getRenderer(), c);
    System::Texture::render(scene, Renderer::SDL::getRenderer(), c);
    System::Light::render(scene, c);
    
    //RENDER DEBUG
    if (c.render_collision_boxes)
        System::Collider::render(scene, Renderer::SDL::getRenderer(), c);
    
    //BACKGROUND
    SDL_SetRenderDrawColor(Renderer::SDL::getRenderer(), c.background_color[0], c.background_color[1], c.background_color[2], c.background_color[3]);
    
    //RENDER FRAMEBUFFER
#ifdef USE_OPENGL
    Renderer::GL::render(c);
#endif
    
    //RENDER GUI
    if (c.enable_gui)
        Gui::render();
    
    //PRESENT
#ifdef USE_OPENGL
    Renderer::GL::present(window);
#else
    Renderer::SDL::present(c, window, palette_tex);
#endif
}


void Graphics::destroy() {
#ifdef USE_OPENGL
    Renderer::GL::destroy();
#else
    Renderer::SDL::destroy(palette_tex);
#endif
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
