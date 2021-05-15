//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "config.h"

namespace Verse::System::Collider
{
    std::vector<EntityID> checkObjectCollisions(EntityID eid, Scene &scene);
    bool checkTilemapCollision(Component::Collider* test_col, Component::Collider* tile_col, Component::Tilemap* tilemap);
    bool checkCollisions(EntityID eid, Scene &scene);
    void render(Scene &scene, Config &c);
}
