//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include <vector>

namespace Verse::Component
{
    struct Texture {
        ui8* tex;
        ui32 tex_id;
        str res;
        
        Rect2 transform;
        
        std::vector<Vec2> offset;
        std::vector<int> layer;
        bool is_reversed = false;
    };
}
