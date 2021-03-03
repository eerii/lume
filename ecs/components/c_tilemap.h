//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include <SDL2/SDL.h>
#include "dtypes.h"

namespace Verse::Component
{
    struct Tilemap {
        std::vector<std::vector<ui8>> tiles;
        SDL_Texture* tex;
        Vec2 tex_size;
        Vec2 pos;
    };
}
