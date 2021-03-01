//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include "log.h"
#include "shader.h"
#include "config.h"

namespace Verse::Graphics
{
    void present(Config &c, SDL_Renderer *renderer, SDL_Window *window,
                 SDL_Texture *render_target, SDL_Texture *palette_tex, ui8 pid);

    void handlePaletteTransition(Config &c);
}
