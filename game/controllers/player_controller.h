//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "config.h"
#include "state_machines_list.h"

#define EPSILON 5
#define COYOTE_TIMEOUT 100
#define GRACE_TIMEOUT 150

typedef bool (*actor_move_func)(Verse::Config&, Verse::EntityID, Verse::State::StateType);

namespace Verse::Controller::Player
{
    bool controller(Config &c, EntityID eid, actor_move_func actor_move);
    void move(Config &c, bool right);
    void jump();
    void releaseJump();
    void respawn(Config &c);
    bool checkGroundDown(Config &c, EntityID eid, int down = 1);

    str getCurrentJumpState();
    str getCurrentMoveState();
    void resetState(Config &c);
}
