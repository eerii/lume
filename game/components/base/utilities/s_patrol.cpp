//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_patrol.h"

using namespace Verse;

void System::Patrol::load(EntityID eid, YAML::Node &entity, Scene *s, Config &c) {
    Component::Patrol* patrol = s->addComponent<Component::Patrol>(eid);
    if (entity["patrol"]["points"]) {
        if (entity["patrol"]["points"].IsSequence())
            patrol->points = entity["patrol"]["points"].as<std::vector<Vec2>>();
        if (entity["patrol"]["points"].IsScalar())
            patrol->points = {entity["patrol"]["points"].as<Vec2>()};
    }
}
