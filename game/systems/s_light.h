//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include <glm/glm.hpp>

#include "dtypes.h"
#include "log.h"
#include "ecs.h"
#include "scene.h"
#include "config.h"

namespace Verse::System::Light
{
    void render(Scene &scene, Config &c);
    std::vector<glm::vec4> getLight();
}
