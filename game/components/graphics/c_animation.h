//project fresa, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"

namespace Fresa::Component
{
    struct AnimationFrame {
        std::vector<ui16> index;
        std::vector<ui16> ms;
    };

    struct Animation {
        str curr_key;
        str target_key;
        ui8 curr_frame = 0;
        
        ui32 timer;
        
        std::vector<str> queue;
        ui16 queue_left;
        bool change_now = false;
        
        std::map<str, AnimationFrame> frames;
        
        ui16 size;
    };
}
