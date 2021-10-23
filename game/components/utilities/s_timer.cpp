//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_timer.h"

#include "gui.h"
#include "gui_types.h"

using namespace Verse;

void System::Timer::load(EntityID eid, YAML::Node &entity, Scene *s, Config &c) {
    Component::Timer* timer = s->addComponent<Component::Timer>(eid);
    if (entity["timer"]["ms"]) {
        if (entity["timer"]["ms"].IsSequence())
            timer->ms = entity["timer"]["ms"].as<std::vector<ui16>>();
        else
            timer->ms = {entity["timer"]["ms"].as<ui16>()};
    }
    timer->tid = std::vector<ui32>(timer->ms.size());
}

//TODO: Timer save

void System::Timer::gui(Config &c, EntityID eid) {
#ifndef DISABLE_GUI
    Component::Timer* timer = c.active_scene->getComponent<Component::Timer>(eid);
    if (timer == nullptr)
        return;
    
    for (int i = 0; i < timer->ms.size(); i++) {
        float delay = (float)timer->ms[i] / 1000.0f;
        Verse::Gui::draw_float(delay, "delay", eid + i);
        timer->ms[i] = (ui16)(delay * 1000.0f);
        ImGui::TableNextRow();
    }
#endif
}
