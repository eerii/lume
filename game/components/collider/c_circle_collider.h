//project fresa, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"

namespace Fresa::Component
{
    struct CircleCollider {
        Vec2<> pos;
        ui16 radius;
        ui8 layer;
        bool is_colliding;
    };
}
