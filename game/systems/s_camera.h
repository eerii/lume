//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include "log.h"
#include "config.h"
#include "ecs.h"
#include "scene.h"
#include "c_camera.h"

namespace Verse::System::Camera
{
    void init(Component::Camera* camera);
    void update(Config &c, Scene &s);
    void updatePoints(Config &c, Vec2f pos);
}
