//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "config.h"

namespace Verse::System::Actor
{
    void update(Config &c, Scene &scene);
    void move(Config &c, Scene &scene, EntityID eid);
}
