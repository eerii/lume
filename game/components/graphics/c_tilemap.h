//project fresa, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "r_dtypes.h"

namespace Fresa::Component
{
    struct Tilemap {
        std::vector<str> res;
        std::vector<Fresa::Graphics::TextureData> tex_data;
        
        str tile_res;
        std::vector<std::vector<ui8>> tiles;
        
        Vec2<> tex_size;
        Vec2<> pos;
        
        int layer;
        
        std::vector<std::vector<float>> vert;
        
        Tilemap() {
            tiles.push_back({0});
            res.push_back("");
        }
    };
}
