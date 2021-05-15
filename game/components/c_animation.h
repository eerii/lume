//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include <vector>
#include <map>

namespace Verse::Component
{
    struct Animation {
        str curr_key;
        ui8 curr_frame = 0;
        std::map<str, std::vector<ui16>> frames;
        
        ui16 size;
        ui16 calculate_size() const {
            ui16 s = 0;
            for (auto const& [k, f] : frames) { s += f.size(); }
            return s;
        };
    };
}
