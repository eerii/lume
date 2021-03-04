//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include "log.h"
#include "config.h"

namespace Verse::Graphics::Renderer::SDL
{
    SDL_Renderer* create(Config &c, SDL_Window* window, ui8 &pid);

    void clear(Config &c, SDL_Renderer* renderer);
    void present(Config &c, SDL_Renderer* renderer, SDL_Window* window, SDL_Texture* palette_tex, ui8 &pid);
}
