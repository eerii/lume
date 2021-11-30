//project fresa, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include "reflection.h"

namespace Fresa::Component
{
    struct Collider {
        Serialize(Collider, transform, layer, is_colliding)
        Rect2<> transform;
        ui8 layer;
        bool is_colliding;
    };
}
