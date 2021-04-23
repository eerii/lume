//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"

namespace Verse::Component
{
    struct Camera {
        Vec2 centre;
        Vec2 vel;
        float l,r,t,b;
    };
}
