//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include <functional>
#include <bitset>

#define MAX_COLLISION_LAYERS 16

namespace Verse::Component
{
    struct Actor {
        Vec2f vel;
        Vec2f remainder;
        
        int max_move_speed;
        int max_fall_speed;
        
        int acc_ground;
        float friction_ground;
        
        bool has_gravity = true;
        bool is_on_ground = false;
        
        std::function<bool()> controller;
        
        std::bitset<MAX_COLLISION_LAYERS> collision_mask;
    };
}
