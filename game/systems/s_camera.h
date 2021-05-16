//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "config.h"

namespace Verse::System::Camera
{
    void init(Component::Camera* camera);
    void update(Config &c);
    void updatePoints(Config &c, Vec2f pos);
}
