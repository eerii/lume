//project fresa, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "config.h"
#include "state_machines_list.h"

namespace Fresa::Controller::FallingPlatform
{
    bool controller(Config &c, EntityID eid);
    bool checkActorAbove(Config &c, EntityID eid);
    bool checkGroundBelow(Config &c, EntityID eid);
}
