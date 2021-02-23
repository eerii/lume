//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "dtypes.h"
#include "log.h"

namespace Verse::Platform
{
    bool init();

    static SDL_Event event;
    bool handle_events(SDL_Event* event);
}
