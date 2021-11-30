//project fresa, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"

namespace Fresa::Component
{
    struct Player {
        float light_strength = 150.0f;
        float recover_light_speed = 30.0f;
        float lose_light_speed = 5.0f;
        bool recovering_light = false;
    };
}
