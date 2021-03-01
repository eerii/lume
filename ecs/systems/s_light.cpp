//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_light.h"
#include "shader.h"

using namespace Verse;

namespace {
    SDL_Surface* light_surface;
    SDL_Texture* light_tex;

    SDL_Rect src;
    SDL_Rect dst;
}

void System::Light::init(SDL_Renderer* renderer, Config &c) {
    
    //BITWISE COLOR MASKS
    ui32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
    
    //SURFACE
    light_surface = SDL_CreateRGBSurface(0, c.window_size.x, c.window_size.y, 32, rmask, gmask, bmask, amask);
    
    if (light_surface == NULL)
        log::error("Creating the light surface failed: %s", SDL_GetError());
    if (light_surface->pixels == nullptr)
        log::error("The light surface has no pixels");
    
    //EVERY PIXEL TO WHITE
    clean();
    
    //TEXTURE
    light_tex = SDL_CreateTextureFromSurface(renderer, light_surface);
    
    //RECTS
    src = Rect(Vec2(0,0), c.window_size).toSDL();
    dst = Rect(Vec2(0,0), c.window_size).toSDL();
}

void System::Light::render(ui8 pid) {
    //Change lighting
    
    glActiveTexture(GL_TEXTURE2);
    SDL_GL_BindTexture(light_tex, NULL, NULL);
    glUniform1i(glGetUniformLocation(pid, "light"), 2);
}

void System::Light::clean() {
    for (int j = 0; j < light_surface->h; j++) {
        for (int i = 0; i < light_surface->w; i++) {
            ui8 *color = (ui8*)light_surface->pixels + j * light_surface->pitch + i * 4;
            
            color[0] = color[1] = color[2] = (i > light_surface->w / 2) ? 255 : 0;
            color[3] = 255;
        }
    }
}
