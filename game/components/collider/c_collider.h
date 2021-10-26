//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include "reflection.h"
#include <vector>

namespace Verse::Component
{
    struct Collider {
        Serialize(Collider, transform, layer, is_colliding)
        Rect2<int> transform;
        ui8 layer;
        bool is_colliding;
    };
}
