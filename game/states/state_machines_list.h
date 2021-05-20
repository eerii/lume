//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "sm_door_test.h"
#include "sm_player_jump.h"
#include <variant>

#define NAMESPACES using namespace Door; \
                   using namespace Player;

namespace Verse::State
{

    struct PlayerStates {
        int coyote_timeout = 10;
        int grace_timeout = 10;
        
        PlayerStates() = default;
        PlayerStates(int p_coyote, int p_grace) : coyote_timeout(p_coyote), grace_timeout(p_grace) {}
        
        State::Player::JumpSM jump{State::Player::GroundedState(),
                                   State::Player::JumpingState(),
                                   State::Player::FallingState(),
                                   State::Player::FallingCoyoteState(coyote_timeout),
                                   State::Player::FallingButJumpingState(grace_timeout)};
    };

    struct NoStates {};

    using StateType = std::variant<bool, State::PlayerStates*>;

}
