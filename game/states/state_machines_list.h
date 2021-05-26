//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "sm_door_test.h"
#include "sm_player_jump.h"
#include "sm_player_move.h"
#include <variant>

#define NAMESPACES using namespace Door; \
                   using namespace Player;

namespace Verse::State
{

    struct PlayerStates {
        ui16 coyote_timeout = 100;
        ui16 grace_timeout = 100;
        ui16 max_speed = 0;
        ui16 min_speed = 0;
        
        PlayerStates() = default;
        PlayerStates(ui16 p_coyote, ui16 p_grace, ui16 p_max, ui16 p_min) :
            coyote_timeout(p_coyote),
            grace_timeout(p_grace),
            max_speed(p_max),
            min_speed(p_min)
        {}
        
        Player::JumpSM jump{Player::GroundedState(),
                            Player::JumpingState(),
                            Player::FallingState(),
                            Player::FallingCoyoteState(coyote_timeout),
                            Player::FallingButJumpingState(grace_timeout),
                            Player::FallingFasterState(),
                            Player::FallingFasterButJumpingState(grace_timeout),
                            Player::CrouchingState()};
        
        Player::MoveSM move{Player::IdleState(),
                            Player::AcceleratingState(max_speed),
                            Player::MovingState(),
                            Player::DeceleratingState(min_speed)};
    };

    using StateType = std::variant<bool, State::PlayerStates*>;

}
