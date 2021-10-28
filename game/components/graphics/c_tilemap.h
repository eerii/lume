//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include "r_texturedata.h"

namespace Verse::Component
{
    struct Tilemap {
        std::vector<str> res;
        std::vector<Verse::Graphics::TextureData> tex_data;
        
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
