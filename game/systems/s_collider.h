//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "config.h"

namespace Verse::System::Collider
{
    std::vector<EntityID> checkObjectCollisions(Config &c, EntityID eid);
    bool checkTilemapCollision(Component::Collider* test_col, Component::Collider* tile_col, Component::Tilemap* tilemap);
    int checkCollisions(Config &c, EntityID eid);
    void render(Config &c);
}
