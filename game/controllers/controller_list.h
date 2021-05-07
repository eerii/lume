//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "player_controller.h"
#include "free_controller.h"

#define PLAYER_CONTROLLER [&s, eid]() -> bool {return Controller::Player::controller(s, eid);};
#define FREE_ACTOR_CONTROLLER [&s, eid]() -> bool {return Controller::Free::controller(s, eid);};
