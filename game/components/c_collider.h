//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"

namespace Verse::Component
{
    struct Collider {
        Rect2 transform;
        ui8 layer;
        bool is_colliding;
    };

    enum ColliderLayers {
        GROUND,
        ACTORS,
        EVENT
    };
}
