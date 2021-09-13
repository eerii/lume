//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"

#include <variant>

#include "sm_door_test.h"
#include "sm_player_jump.h"
#include "sm_player_move.h"

#define NAMESPACES using namespace Door; \
                   using namespace Player;

#define jump_state std::get<State::Player::JumpSM>(state->states[state->index["jump"]])
#define move_state std::get<State::Player::MoveSM>(state->states[state->index["move"]])

#define STATE_NAMES { \
                        {"jump", false}, \
                        {"move", false} \
                    }

namespace Verse
{
    using StateType = std::variant<State::Player::JumpSM,
                                   State::Player::MoveSM>;
}

namespace Verse::State
{
    template <typename State>
    str getCurrentState(State state) {
        return CURR_STATE(state);
    }
}
