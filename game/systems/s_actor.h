//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "config.h"
#include "state_machines_list.h"

namespace Verse::System::Actor
{
    void update(Config &c);
    bool move(Config &c, EntityID eid, State::StateType state);
}
