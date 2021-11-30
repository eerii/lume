//project fresa, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "r_dtypes.h"

namespace Fresa::Component
{
    struct Texture {
        Fresa::Graphics::TextureData data;
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
