//project fresa, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "r_dtypes.h"

namespace Fresa::Component
{
    struct Noise {
        Vec2<> offset;
        
        Fresa::Graphics::TextureData mask_tex;
        Fresa::Graphics::TextureData noise_tex;
        std::vector<ui8> noise_data;
        Vec2<> size;
        
        Vec2<> dir;
        ui8 fps;
        
        float freq;
        int levels = 4;
        
        float noise_time = 1000;
        float noise_offset = 0;
        
        bool enabled = true;
    };
}
