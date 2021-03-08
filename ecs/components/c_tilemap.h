//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include <vector>

#include "dtypes.h"

namespace Verse::Component
{
    struct Tilemap {
        ui8* tex;
        ui32 tex_id;
        
        std::vector<std::vector<ui8>> tiles;
        
        Vec2 tex_size;
        Vec2 pos;
    };
}
