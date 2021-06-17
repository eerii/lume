//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_timer.h"

using namespace Verse;

void System::Timer::load(EntityID eid, YAML::Node &entity, Scene *s, Config &c) {
    Component::Timer* timer = s->addComponent<Component::Timer>(eid);
    if (entity["timer"]["ms"])
        timer->ms = entity["timer"]["ms"].as<ui16>();
}
