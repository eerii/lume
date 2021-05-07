//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"

namespace Verse::Component
{
    struct Camera {
        Vec2f pos;
        Vec2f vel;
        
        Rect2 bounds;
        Vec2 focus_size;
        float l,r,t,b;
        
        float look_ahead;
        float la_speed;
        
        std::function<bool()> controller;
    };
}
