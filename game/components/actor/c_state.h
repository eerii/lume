//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include <map>
#include "state_machines_list.h"

namespace Verse::Component
{
    struct State {
        std::vector<Verse::StateType> states;
        std::map<str, bool> has_state = STATE_NAMES;
        std::map<str, ui8> index;
    };
}
