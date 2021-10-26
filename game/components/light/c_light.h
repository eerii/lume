//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include <glm/glm.hpp>

namespace Verse::Component
{
    struct Light {
        Vec2<int> pos;
        float radius;
        
        float period = 5;
        float variation = 5;
        float center_radius = 0.25;
        
        glm::vec4 render_data;
    };
}
