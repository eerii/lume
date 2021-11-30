//project fresa, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"

#include <variant>

#include "sm_door_test.h"
#include "sm_player_jump.h"
#include "sm_player_move.h"
#include "sm_checkpoint.h"

#define NAMESPACES using namespace Door; \
                   using namespace Player; \
                   using namespace Checkpoint;

#define jump_state std::get<State::Player::JumpSM>(state->states[state->index["jump"]])
#define move_state std::get<State::Player::MoveSM>(state->states[state->index["move"]])
#define checkpoint_state std::get<State::Checkpoint::CheckpointSM>(state->states[state->index["checkpoint"]])

#define STATE_NAMES { \
                        {"jump", false}, \
                        {"move", false}, \
                        {"checkpoint", false} \
                    }

namespace Fresa
{
    using StateType = std::variant<State::Player::JumpSM,
                                   State::Player::MoveSM,
                                   State::Checkpoint::CheckpointSM>;
}

namespace Fresa::State
{
    template <typename State>
    str getCurrentState(State state) {
        return CURR_STATE(state);
    }
}
