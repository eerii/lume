//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"

namespace Verse::Component
{
    struct Texture {
        ui8* tex;
        ui32 tex_id;
        
        Rect2 transform;
        
        Vec2 offset = Vec2(0,0);
        bool is_reversed = false;
    };
}
