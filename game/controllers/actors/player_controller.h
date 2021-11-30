//project fresa, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "config.h"
#include "state_machines_list.h"

#define EPSILON 5
#define COYOTE_TIMEOUT 100
#define GRACE_TIMEOUT 150

namespace Fresa::Controller::Player
{
    bool controller(Config &c, EntityID eid);
    void move(Config &c, bool right);
    void jump(Config &c, EntityID eid);
    void releaseJump();
    void down(Config &c, EntityID eid);
    void respawn(Config &c);
    bool checkGroundDown(Config &c, EntityID eid, int down = 1);
    Vec2<float> getMovingPlatformVelocity(Config &c, EntityID eid);
}
