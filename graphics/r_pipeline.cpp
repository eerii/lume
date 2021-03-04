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
    
    ui8 pid;

    //Deprecated
    SDL_Renderer *sdl_renderer;
}

void Graphics::init(Config &c) {
    
    //MODERN OPENGL
    #ifdef USE_OPENGL
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
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
    Renderer::createRenderer(pid);
    #else
    sdl_renderer = Renderer::SDL::create(c, window, pid);
    Graphics::linkRendererToTexture(sdl_renderer);
    #endif
    
    //IMGUI
    ImGui::CreateContext();
    #ifndef USE_OPENGL
    ImGuiSDL::Initialize(sdl_renderer, c.window_size.x, c.window_size.y);
    #endif
    
    //PALETTE
    palette_tex = loadTexture("res/graphics/palette_multi.png");
    
    //LIGHT
    System::Light::init(sdl_renderer, c);
}


void Graphics::render(Scene &scene, Config &c, ui16 fps) {
    //PRERENDER GUI
    prerender(scene, c, fps);
    
    //CLEAR
#ifdef USE_OPENGL
    
#else
    Renderer::SDL::clear(c, sdl_renderer);
#endif
    
    //RENDER TEXTURES
    System::Tilemap::render(scene, sdl_renderer, c);
    System::Texture::render(scene, sdl_renderer, c);
    System::Light::render(scene, c);
    
    //RENDER DEBUG
    if (c.render_collision_boxes)
        System::Collider::render(scene, sdl_renderer, c);
    
    //BACKGROUND
    SDL_SetRenderDrawColor(sdl_renderer, c.background_color[0], c.background_color[1], c.background_color[2], c.background_color[3]);
    
    //RENDER GUI
    if (c.enable_gui)
        Gui::render();
    
    //PRESENT
#ifdef USE_OPENGL
    
#else
    Renderer::SDL::present(c, sdl_renderer, window, palette_tex, pid);
#endif
}


void Graphics::prerender(Scene &scene, Config &c, ui16 fps) {
    if (c.enable_gui)
        Gui::prerender(scene, c, fps);
}


void Graphics::destroy() {
#ifdef USE_OPENGL
    
#else
    //SDL_DestroyTexture(palette_tex);
    //SDL_DestroyTexture(render_target);
    
    SDL_DestroyRenderer(sdl_renderer);
    
    SDL_DestroyWindow(window);
#endif
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
