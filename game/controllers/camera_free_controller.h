//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"

#include "scene.h"
#include "config.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace Verse::Controller::Camera::Free
{
    bool controller(Scene &s, Config &c, EntityID eid);
}
