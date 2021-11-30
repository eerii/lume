//project fresa, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "config.h"
#include "serialization.h"

namespace Fresa::System::Text
{
    void render(Config &c);
    
    void load(EntityID eid, YAML::Node &entity, Scene *s, Config &c);
    void save(Component::Text* text, str path, std::vector<str> &key);
    void gui(Config &c, EntityID eid);
}
