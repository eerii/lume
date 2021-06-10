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
    void jump(Config &c, EntityID eid);
    void releaseJump();
    void down(Config &c, EntityID eid);
    void respawn(Config &c);
    bool checkGroundDown(Config &c, EntityID eid, int down = 1);
    Vec2f getMovingPlatformVelocity(Config &c, EntityID eid);

    str getCurrentJumpState();
    str getCurrentMoveState();
    void resetState(Config &c);
}
