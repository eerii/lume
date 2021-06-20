//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "config.h"
#include "state_machines_list.h"

namespace Verse::Controller::FallingPlatform
{
    bool controller(Config &c, EntityID eid);
    bool checkActorAbove(Config &c, EntityID eid);
    bool checkGroundBelow(Config &c, EntityID eid);
}
