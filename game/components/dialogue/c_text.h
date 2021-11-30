//project fresa, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "r_dtypes.h"
#include "stb_truetype_fresa.h"

namespace Fresa
{
    struct FontInfo {
        str path;
        std::vector<ui8> data;
        
        ui64 font_size;
        stbtt_fontinfo info;
    };
}

namespace Fresa::Component
{
    struct Text {
        FontInfo* font;
        
        Vec2<> bitmap_size;
        
        Fresa::Graphics::TextureData tex_data;
        Rect2<> transform;
        int layer;
        
        int line_height;
        
        bool solid_color = true;
        float r, g, b;
        
        str text;
        str previous_text;
    };
}
