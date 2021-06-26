//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"

namespace Verse::Component
{
    struct Noise {
        Vec2 offset;
        
        ui32 noise_tex;
        std::vector<ui8> noise_data;
        Vec2 size;
        
        Vec2 dir;
        ui8 fps;
        
        float freq;
        int levels = 4;
        
        float noise_time = 1000;
        float noise_offset = 0;
    };
}
