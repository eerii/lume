//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

//TODO: Remove in favour of my texture loading
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "dtypes.h"
#include "log.h"

namespace Verse::Graphics
{
    void linkRendererToTexture(SDL_Renderer* r); //Deprecate in favour of custom renderer
    SDL_Texture* loadTexture(str path);
    SDL_Surface* loadSurface(str path);
}
