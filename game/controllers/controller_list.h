//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "player_controller.h"
#include "free_actor_controller.h"

#define PLAYER_CONTROLLER [&c, eid, move]() -> bool {return Controller::Player::controller(c, eid, move);};
#define FREE_ACTOR_CONTROLLER [&c, eid, move]() -> bool {return Controller::FreeActor::controller(c, eid, move);};

#define PLAYER_DAMAGE [&c]() -> void {return Controller::Player::respawn(c);};

#include "camera_actor_controller.h"
#include "camera_free_controller.h"

#define CAMERA_ACTOR_CONTROLLER [&c, eid]() -> bool {return Controller::Camera::Actor::controller(c, eid);};
#define CAMERA_FREE_CONTROLLER [&c, eid]() -> bool {return Controller::Camera::Free::controller(c, eid);};

#include "moving_platform_controller.h"
#include "falling_platform_controller.h"

#define MOVING_PLATFORM_CONTROLLER [&c, eid, move]() -> bool {return Controller::MovingPlatform::controller(c, eid, move);};
#define FALLING_PLATFORM_CONTROLLER [&c, eid, move]() -> bool {return Controller::FallingPlatform::controller(c, eid, move);};
