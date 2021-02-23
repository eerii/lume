//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include "log.h"

#include "scene.h"

namespace Verse::System::Actor
{
    void init();
    void update(Scene &scene);
    void move(Scene &scene, EntityID eid);

    static float gravity;
}
