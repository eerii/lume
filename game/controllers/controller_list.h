//project fresa, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "config.h"

#include "player_controller.h"
#include "free_actor_controller.h"

#include "moving_platform_controller.h"
#include "falling_platform_controller.h"
#include "switch_platform_controller.h"
#include "disable_platform_controller.h"

#include "camera_actor_controller.h"
#include "camera_free_controller.h"

using namespace Fresa;

namespace Fresa
{
    static std::map<str, std::function<bool(Config &c, EntityID eid)>> controllers = {
        {"player", Controller::Player::controller},
        {"free_actor", Controller::FreeActor::controller},
        {"moving_platform", Controller::MovingPlatform::controller},
        {"falling_platform", Controller::FallingPlatform::controller},
        {"switch_platform", Controller::SwitchPlatform::controller},
        {"disable_platform", Controller::DisablePlatform::controller},
    };

    static std::map<str, std::function<void(Config &c)>> damage_controllers = {
        {"player", Controller::Player::respawn},
    };

    static std::map<str, std::function<bool(Config &c, EntityID eid)>> camera_controllers = {
        {"actor", Controller::Camera::Actor::controller},
        {"free", Controller::Camera::Free::controller},
    };
}
