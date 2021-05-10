//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "player_controller.h"
#include "free_actor_controller.h"

#define PLAYER_CONTROLLER [&s, eid]() -> bool {return Controller::Player::controller(s, eid);};
#define FREE_ACTOR_CONTROLLER [&s, eid]() -> bool {return Controller::FreeActor::controller(s, eid);};

#include "camera_actor_controller.h"

#define CAMERA_ACTOR_CONTROLLER [&s, &c, eid]() -> bool {return Controller::Camera::Actor::controller(s, c, eid);};
