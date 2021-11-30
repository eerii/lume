//project fresa, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "config.h"
#include "serialization.h"

namespace Fresa::System::SceneTransition
{
    void handle(Config &c, Component::SceneTransition* transition);
    void handle(Config &c, Scene* new_scene, Vec2<> new_pos);

    void load(EntityID eid, YAML::Node &entity, Scene *s, Config &c);
    void gui(Config &c, EntityID eid);
}
