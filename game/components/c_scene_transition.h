//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"

namespace Verse { struct Scene; }

namespace Verse::Component
{
    struct SceneTransition {
        Scene* to_scene;
        Vec2 to_pos;
    };
}
