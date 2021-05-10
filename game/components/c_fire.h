//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"

namespace Verse::Component
{
    struct Fire {
        Rect2 transform;
        Vec2 offset = Vec2(0, 0);
        
        ui32 flame_tex, p_tex;//, w_tex;
        ui8* p_data;
        //ui8* w_data;
        
        Vec2 dir;
        ui8 fps;
        
        float freq;
        ui8 octaves;
        ui32 seed;
    };
}
