//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"

namespace Verse::Component
{
    struct Patrol {
        std::vector<Vec2> points;
        int current;
    };
}
