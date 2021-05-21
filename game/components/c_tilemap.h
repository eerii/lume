//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include <vector>
#include <glm/glm.hpp>

namespace Verse::Component
{
    struct Tilemap {
        std::vector<ui32> tex_id;
        
        std::vector<std::vector<ui8>> tiles;
        
        Vec2 tex_size;
        Vec2 pos;
        
        int layer;
        
        std::vector<std::vector<std::array<float, 24>>> vert;
    };
}
