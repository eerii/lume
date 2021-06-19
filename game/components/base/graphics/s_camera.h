//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "config.h"
#include "serialization.h"

namespace Verse::System::Camera
{
    void init(Component::Camera* camera);
    void update(Config &c);
    void shake(Config &c, ui16 ms, float strength);

    void load(EntityID eid, YAML::Node &entity, Scene *s, Config &c);
    void gui(Config &c, EntityID eid);
}
