//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include <vector>

namespace Verse::Component
{
    struct CircleCollider {
        Vec2<int> pos;
        ui16 radius;
        ui8 layer;
        bool is_colliding;
    };
}
