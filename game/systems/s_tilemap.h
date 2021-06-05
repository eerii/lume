//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "config.h"

namespace Verse::System::Tilemap
{
    std::vector<std::vector<ui8>> load(str path);
    void init(Config &c);
    void createVertices(Config &c, Component::Tilemap* tmap);
    void render(Config &c);
    Vec2 calculateSize(Component::Tilemap* tmap);

    void renderEditor(Config &c);
}
