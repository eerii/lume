//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "config.h"
#include "serialization.h"

namespace Verse::System::Camera
{
    void prerender(Config &c);
    void update(Config &c);
    void shake(Config &c, ui16 ms, float strength);

    void load(EntityID eid, YAML::Node &entity, Scene *s, Config &c);
    void save(Component::Camera* cam, str path, std::vector<str> &key, Scene *s);
    void gui(Config &c, EntityID eid);
}
