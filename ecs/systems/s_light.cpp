//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_light.h"
#include "r_opengl.h"
#include "time.h"

#define LIGHT_PERIOD 5
#define LIGHT_VARIATION 5
#define LIGHT_CENTER_RADIUS 0.2

using namespace Verse;

namespace {
    SDL_Surface* light_surface;
    SDL_Texture* light_tex;

    SDL_Rect src;
    SDL_Rect dst;

    std::vector<EntityID> light_entities;
    std::vector<glm::vec4> light_sources;
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
    
    //RANDOM SEED
    srand((ui32)Time::current);
}

void System::Light::render(Scene &scene, Config &c) {
    for (EntityID e : SceneView<Component::Light>(scene)) {
        Component::Light* light = scene.getComponent<Component::Light>(e);
        Component::Texture* tex = scene.getComponent<Component::Texture>(e);
        
        std::vector<EntityID>::iterator it = std::find(light_entities.begin(), light_entities.end(), e);
        if (it != light_entities.end()) {
            long i = it - light_entities.begin();
            
            light_sources[i][0] = light->pos.x;
            light_sources[i][1] = light->pos.y;
            if (tex != nullptr) { //This is to render the light relative to the texture
                light_sources[i][0] += tex->transform.pos.x;
                light_sources[i][1] += tex->transform.pos.y;
            }
            
            light_sources[i][0] /= c.resolution.x;
            light_sources[i][1] /= c.resolution.y;
            
            light_sources[i][2] = (light->radius + sin(Time::current * 0.001 * LIGHT_PERIOD) * LIGHT_VARIATION) / c.resolution.y;
            light_sources[i][3] = light_sources[i][2] * LIGHT_CENTER_RADIUS;
        } else {
            light_entities.push_back(e);
            light_sources.push_back({light->pos.x, light->pos.y, light->radius, light->radius * LIGHT_CENTER_RADIUS});
        }
    }
}

std::vector<glm::vec4> System::Light::getLight() {
    return light_sources;
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
