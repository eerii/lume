//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "config.h"
#include "state_machines_list.h"

typedef bool (*actor_move_func)(Verse::Config&, Verse::EntityID, Verse::State::StateType);

namespace Verse::Controller::MovingPlatform
{
    bool controller(Config &c, EntityID eid, actor_move_func move);
}