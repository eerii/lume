//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"

namespace Verse::Component
{
    struct Fire {
        Rect2 transform;
        Vec2 offset;
        Vec2 initial_offset;
        std::vector<int> vertical_offsets = {};
        
        ui32 flame_tex, p_tex;
        ui8* p_data;
        
        Vec2 dir;
        ui8 fps;
        
        float freq;
        ui8 octaves;
        ui32 seed;
        
        int layer;
        
        float noise_time = 1000;
        ui16 noise_offset = 0;
        
        ~Fire() {
            delete[] p_data;
        }
    };
}
