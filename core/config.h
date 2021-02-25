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
        
        bool use_shaders;
        int palette_index;
        int num_palettes;
        
        bool render_collision_boxes;
    };
}
