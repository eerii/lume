//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#ifdef USE_OPENGL
#include "stb_image.h"
#else
#include <SDL2/SDL_image.h>
#endif

#include "dtypes.h"
#include "log.h"
#include "config.h"

namespace Verse::Graphics
{
    ui8* loadTexture(str path, ui32 &tex_id);
}
