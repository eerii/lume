//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"

namespace Verse
{
    struct Config {
        str name;
        str version;
        
        Vec2 resolution;
        Vec2 window_size;
        Vec2 window_padding;
        int render_scale;
        
        bool use_shaders;
        bool use_grayscale;
        int palette_index;
        int num_palettes;
        
        int background_color[4];
        
        bool render_collision_boxes;
    };
}
