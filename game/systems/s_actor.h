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
    ui8 collisions(Config &c, EntityID eid, State::StateType state, bool perform_actions = true);
    void checkMovingPlatform(Config &c, EntityID eid);

    enum Colliding {
        Solid,
        Transparent,
        Exit
    };
}
