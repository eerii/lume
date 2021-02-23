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
        static SDL_Window *window;
        static SDL_Renderer *renderer;
    
        void init();
    
        void clear();
        void render(Scene &scene, Config &c);
        void display();
    
        void calculateRefreshRate();
        static int refreshRate = 60;
    
        SDL_Texture* loadTexture(std::string path);
    }
}
