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
        
        str flame_tex_res;
        ui32 flame_tex, p_tex, g_tex;
        ui8* p_data;
        
        Vec2 dir;
        ui8 fps;
        
        float freq;
        int levels = 4;
        int layer;
        
        float noise_time = 1000;
        float noise_offset = 0;
        
        ~Fire() {
            delete[] p_data;
        }
    };
}
