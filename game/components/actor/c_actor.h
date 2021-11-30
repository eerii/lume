//project fresa, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include <functional>
#include <bitset>

#define MAX_COLLISION_LAYERS 16

namespace Fresa::Component
{
    struct Actor {
        Vec2<float> vel;
        Vec2<float> extra_vel;
        Vec2<float> remainder;
        
        int max_move_speed; //TODO: Change to vec2
        int max_fall_speed;
        
        int acc_ground;
        int friction_ground;
        int friction_air;
        int friction_extra;
        
        bool has_gravity = true;
        
        std::function<bool()> controller;
        std::function<void()> damage;
        str current_controller;
        
        std::bitset<MAX_COLLISION_LAYERS> collision_mask;
    };
}
