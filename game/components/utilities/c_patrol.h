//project fresa, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"

namespace Fresa::Component
{
    struct Patrol {
        std::vector<Vec2<>> points;
        int current;
    };
}
