//project fresa, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "config.h"

namespace Fresa::Controller::Camera::Actor
{
    bool controller(Config &c, EntityID eid);
    void checkBounds(Config &c, Component::Camera* cam);
    void lookahead(Config &c, Component::Camera* cam, int input);
}
