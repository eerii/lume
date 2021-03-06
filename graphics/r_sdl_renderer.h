//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include "log.h"
#include "config.h"

namespace Verse::Graphics::Renderer::SDL
{
    void create(Config &c, SDL_Window* window);
    SDL_Renderer* getRenderer();

    void clear(Config &c);
    void present(Config &c, SDL_Window* window, SDL_Texture* palette_tex);
    void destroy(SDL_Texture* palette_tex);
}
