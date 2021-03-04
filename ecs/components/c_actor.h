//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include <functional>
#include "dtypes.h"

namespace Verse::Component
{
    struct Actor {
        Vec2 vel;
        Vec2 remainder;
        
        int max_move_speed;
        int max_fall_speed;
        
        int acc_ground;
        float friction_ground;
        
        bool has_gravity = true;
        bool is_on_ground = false;
        
        std::function<bool()> controller;
    };
}