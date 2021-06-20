//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_timer.h"

#include "gui.h"
#include "gui_types.h"

using namespace Verse;

void System::Timer::load(EntityID eid, YAML::Node &entity, Scene *s, Config &c) {
    Component::Timer* timer = s->addComponent<Component::Timer>(eid);
    if (entity["timer"]["ms"])
        timer->ms = entity["timer"]["ms"].as<ui16>();
}

void System::Timer::gui(Config &c, EntityID eid) {
#ifndef DISABLE_GUI
    Component::Timer* timer = c.active_scene->getComponent<Component::Timer>(eid);
    
    float delay = (float)timer->ms / 1000.0f;
    Verse::Gui::draw_float(delay, "delay", eid);
    timer->ms = (ui16)(delay * 1000.0f);
#endif
}
