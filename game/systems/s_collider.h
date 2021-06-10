//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "config.h"
#include <map>

namespace Verse::System::Collider
{
    using CollisionInfo = std::map<EntityID, std::bitset<MAX_COLLISION_LAYERS>>;
    using CollisionInfoPair = std::pair<EntityID, std::bitset<MAX_COLLISION_LAYERS>>;

    enum Layers {
        Ground,
        Actor,
        Event,
        Scene,
        Water,
        Checkpoint,
        Platform,
        SolidPlatform,
    };

    static std::vector<str> layers_name = {
        "ground",
        "actor",
        "event",
        "scene",
        "water",
        "checkpoint",
        "platform",
        "solid_platform"
    };

    std::vector<EntityID> checkObjectCollisions(Config &c, EntityID eid);
    CollisionInfoPair checkTilemapCollision(Config &c, EntityID eid, Component::Collider* test_col);
    CollisionInfo checkCollisions(Config &c, EntityID eid);
    void render(Config &c);
}
