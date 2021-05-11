//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace Verse::Component
{
    struct Camera {
        Vec2 target_pos;
        Vec2f pos;
        Vec2f vel;
        
        Rect2 bounds;
        Vec2 focus_size;
        float l,r,t,b;
        float scale = 1.0f;
        
        float look_ahead;
        float la_speed;
        
        glm::mat4 m_pixel;
        glm::mat4 m_extra;
        
        std::function<bool()> controller;
    };
}
