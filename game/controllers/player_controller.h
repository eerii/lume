//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include "log.h"
#include "config.h"

#include "scene.h"

namespace Verse::Controller::Player
{
    bool controller(Scene &scene, EntityID eid);
    void jump();
}