//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "config.h"
#include "serialization.h"

namespace Verse::System::Actor
{
    void update(Config &c);
    bool move(Config &c, EntityID eid);
    ui8 collisions(Config &c, EntityID eid, bool perform_actions = true);

    void load(EntityID eid, YAML::Node &entity, Scene *s, Config &c);
    void save(Component::Actor* actor, str path, std::vector<str> &key);
    void gui(Config &c, EntityID eid);

    enum Colliding {
        Solid,
        Transparent,
        Exit
    };
}
