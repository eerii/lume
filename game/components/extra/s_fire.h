//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "config.h"
#include "serialization.h"

namespace Verse::System::Fire
{
    void init(Component::Fire* fire);
    void render(Config &c);

    void load(EntityID eid, YAML::Node &entity, Scene *s, Config &c);
}
