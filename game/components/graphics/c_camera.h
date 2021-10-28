//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include <glm/glm.hpp>

namespace Verse::Component
{
    struct Camera {
        Vec2<float> target_pos;
        Vec2<float> previous_target_pos;
        
        Vec2<float> pos;
        Vec2<float> previous_pos;
        Vec2<float> render_pos;
        
        Rect2<> bounds;
        
        glm::mat4 m_pixel;
        glm::mat4 m_extra;
        
        std::function<bool()> controller;
        str current_controller;
    };
}
