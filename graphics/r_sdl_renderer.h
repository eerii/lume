//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include "log.h"
#include "config.h"

namespace Verse::Graphics::Renderer
{
    SDL_Renderer* createSDLRenderer(Config &c, SDL_Window* window, SDL_Texture* render_target, ui8 &pid);
}
