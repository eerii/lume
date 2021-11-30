//project fresa, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include "state_machines_list.h"

namespace Fresa::Component
{
    struct State {
        std::vector<Fresa::StateType> states;
        std::map<str, bool> has_state = STATE_NAMES;
        std::map<str, ui8> index;
    };
}
