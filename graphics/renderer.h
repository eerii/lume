//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "dtypes.h"
#include "log.h"

#include "scene.h"
#include "ecs.h"
#include "config.h"

#define RENDER_SCALE 4

namespace Verse
{
    namespace Graphics
    {
        void init(Config &c);
    
        void clear(Config &c);
        void render(Scene &scene, Config &c);
        void display(Config &c);
        
        void destroy();
    
        void calculateRefreshRate();
        int getRefreshRate();
    
        SDL_Texture* loadTexture(str path);
        SDL_Surface* loadSurface(str path);
    }
}
