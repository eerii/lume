//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include "r_texturedata.h"

namespace Verse::Component
{
    struct Texture {
        Verse::Graphics::TextureData data;
        str res;
        
        Rect2<> transform;
        bool use_collider_transform;
        
        std::vector<Vec2<>> offset;
        std::vector<int> layer;
        bool is_reversed;
        
        Texture() {
            offset = std::vector<Vec2<>>(1);
            layer = std::vector<int>(1);
        };
    };
}
