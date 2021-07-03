//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include <glm/glm.hpp>

namespace Verse::Component
{
    struct Light {
        Vec2 pos;
        float radius;
        
        glm::vec4 render_data;
    };
}
