//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include "stb_truetype_fresa.h"
#include "r_texturedata.h"

namespace Verse
{
    struct FontInfo {
        str path;
        std::vector<ui8> data;
        
        ui64 font_size;
        stbtt_fontinfo info;
    };
}

namespace Verse::Component
{
    struct Text {
        FontInfo* font;
        
        Vec2<> bitmap_size;
        
        Verse::Graphics::TextureData tex_data;
        Rect2<> transform;
        int layer;
        
        int line_height;
        
        bool solid_color = true;
        float r, g, b;
        
        str text;
        str previous_text;
    };
}
