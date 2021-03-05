//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "r_window.h"
#include "imgui.h"

#ifdef USE_OPENGL
#define W_FLAGS SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
#else
#define W_FLAGS SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
#endif

using namespace Verse;

SDL_Window* Graphics::Window::createWindow(Config &c) {
    SDL_Window* window = SDL_CreateWindow((c.name + " - Version " + c.version).c_str(),
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              c.window_size.x, c.window_size.y, W_FLAGS);
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
    
    return window;
}

void Graphics::Window::onResize(SDL_Event &e, Config &c) {
    if (((float)e.window.data1 / (float)e.window.data2) >= ((float)c.resolution.x / (float)c.resolution.y)) {
        //Longer in the X direction
        c.render_scale = floor((float)e.window.data2 / (float)c.resolution.y);
        
    } else {
        //Longer in the Y direction
        c.render_scale = floor((float)e.window.data1 / (float)c.resolution.x);
    }
    
    c.window_size = Vec2(e.window.data1, e.window.data2);
    
    float w = c.render_scale * c.resolution.x;
    float h = c.render_scale * c.resolution.y;
    float padx = (float)(c.window_size.x - w) / 2.0f;
    float pady = (float)(c.window_size.y - h) / 2.0f;
    c.window_padding = Vec2(padx, pady);
    
    ImGuiIO& imgui_io = ImGui::GetIO();
    imgui_io.DisplaySize.x = static_cast<float>(e.window.data1);
    imgui_io.DisplaySize.y = static_cast<float>(e.window.data2);
}
