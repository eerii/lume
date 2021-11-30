//project fresa, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "config.h"
#include "serialization.h"

namespace Fresa::System::Noise
{
    void init(Config &c, Scene *s, EntityID eid);
    void update(Config &c);
    void render(Config &c);

    void load(EntityID eid, YAML::Node &entity, Scene *s, Config &c);
    void gui(Config &c, EntityID eid);
}
