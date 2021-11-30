//project fresa, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "config.h"
#include "serialization.h"

namespace Fresa::System::Collider
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
        Disabled,
    };

    static std::vector<str> layers_name = {
        "ground",
        "actor",
        "event",
        "scene",
        "water",
        "checkpoint",
        "platform",
        "solid_platform",
        "disabled"
    };

    enum ColliderTypes {
        AABB,
        Circle,
    };

    using CollisionTypeAndID = std::pair<EntityID, ColliderTypes>;

    std::vector<CollisionTypeAndID> checkObjectCollisions(Config &c, EntityID eid);
    CollisionInfoPair checkTilemapCollision(Config &c, EntityID eid, Component::Collider* test_col);
    CollisionInfo checkCollisions(Config &c, EntityID eid);
    void render(Config &c);

    void load(EntityID eid, YAML::Node &entity, struct Scene *s, Config &c);
    void save(Component::Collider* col, str path, std::vector<str> &key, bool tile);
    void gui(Config &c, EntityID eid);

    void load_circle(EntityID eid, YAML::Node &entity, struct Scene *s, Config &c);
    void save_circle(Component::CircleCollider* col, str path, std::vector<str> &key);
    void gui_circle(Config &c, EntityID eid);
}
