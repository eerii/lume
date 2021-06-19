//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "config.h"
#include "serialization.h"

namespace Verse::System::Tilemap
{
    void init(Config &c);
    void createVertices(Config &c, Component::Tilemap* tmap);
    void render(Config &c);
    Vec2 calculateSize(Component::Tilemap* tmap);

    void renderEditor(Config &c);

    std::vector<std::vector<ui8>> loadFromImage(str path);
    void load(EntityID eid, YAML::Node &entity, Scene *s, Config &c);
    void save(Component::Tilemap* tile, str path, std::vector<str> &key);
    void gui(Config &c, EntityID eid);
}
