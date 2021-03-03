//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_light.h"
#include "r_opengl.h"

using namespace Verse;

namespace {
    SDL_Surface* light_surface;
    SDL_Texture* light_tex;

    SDL_Rect src;
    SDL_Rect dst;

    std::vector<EntityID> light_entities;
    std::vector<Vec2> light_sources;
    std::vector<float> light_radius;
    std::vector<float> light_centres;
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

void System::Light::render(Scene &scene, Config &c) {
    for (EntityID e : SceneView<Component::Light>(scene)) {
        Component::Light* light = scene.getComponent<Component::Light>(e);
        Component::Texture* tex = scene.getComponent<Component::Texture>(e);
        
        std::vector<EntityID>::iterator it = std::find(light_entities.begin(), light_entities.end(), e);
        if (it != light_entities.end()) {
            long i = it - light_entities.begin();
            
            light_sources[i] = light->pos * c.render_scale;
            if (tex != nullptr) //This is to render the light relative to the texture
                light_sources[i] += tex->transform.pos * c.render_scale;
            
            light_radius[i] = light->radius * c.render_scale;
            light_centres[i] = light->centre * light_radius[i];
        } else {
            light_entities.push_back(e);
            light_sources.push_back(light->pos);
            light_radius.push_back(light->radius * c.render_scale);
            light_centres.push_back(light->centre);
        }
    }
}

void System::Light::passToShader(ui8 pid, Config &c) {
    glUniform2fv(glGetUniformLocation(pid, "light_source"), (int)light_sources.size(), reinterpret_cast<GLfloat *>(light_sources.data()));
    glUniform1fv(glGetUniformLocation(pid, "light_radius"), (int)light_radius.size(), reinterpret_cast<GLfloat *>(light_radius.data()));
    glUniform1fv(glGetUniformLocation(pid, "light_centre"), (int)light_centres.size(), reinterpret_cast<GLfloat *>(light_centres.data()));
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
