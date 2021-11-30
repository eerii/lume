//project fresa, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"

namespace Fresa { struct Scene; }

namespace Fresa::Component
{
    struct SceneTransition {
        Scene* to_scene;
        Vec2<> to_pos;
        str scene_name;
    };
}
