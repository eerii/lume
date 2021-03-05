//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include "dtypes.h"

namespace Verse::Component
{
    struct Texture {
        Tex* tex;
        ui32 tex_id;
        Rect transform;
        
        Vec2 offset = Vec2(0,0);
        bool is_reversed = false;
        
        std::vector<Vec2> animation;
        int current_animation;
    };
}
