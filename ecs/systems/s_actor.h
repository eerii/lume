//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include "log.h"

#include "ecs.h"
#include "scene.h"

namespace Verse::System::Actor
{
    void update(Config &c, Scene &scene);
    void move(Config &c, Scene &scene, EntityID eid);
}
