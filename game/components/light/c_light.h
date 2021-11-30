//project fresa, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include <glm/glm.hpp>

namespace Fresa::Component
{
    struct Light {
        Vec2<> pos;
        float radius;
        
        float period = 5;
        float variation = 5;
        float center_radius = 0.25;
        
        glm::vec4 render_data;
    };
}
