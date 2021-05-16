//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "config.h"

namespace Verse::System::Light
{
    void render(Config &c);
    std::vector<glm::vec4> getLight();
}
