//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include <vector>
#include <map>

namespace Verse::Component
{
    struct AnimationFrame {
        std::vector<ui16> index;
        std::vector<ui16> ms;
    };

    struct Animation {
        str curr_key;
        ui8 curr_frame = 0;
        
        std::vector<str> queue;
        bool change_now = false;
        
        std::map<str, AnimationFrame> frames;
        
        ui16 size;
    };
}
