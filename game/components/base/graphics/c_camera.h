//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include <glm/glm.hpp>

namespace Verse::Component
{
    struct Camera {
        Vec2 target_pos;
        Vec2 pos;
        Vec2f vel;
        Vec2f remainder;
        
        Rect2 bounds;
        
        glm::mat4 m_pixel;
        glm::mat4 m_extra;
        
        std::function<bool()> controller;
        str current_controller;
    };
}
