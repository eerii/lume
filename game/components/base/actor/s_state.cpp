//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_state.h"

#include "gui.h"
#include "gui_types.h"

using namespace Verse;

void System::State::load(EntityID eid, YAML::Node &entity, Scene *s, Config &c) {
    
}

void System::State::save(Component::State *state, str path, std::vector<str> &key) {
    
}

void System::State::gui(Config &c, EntityID eid) {
#ifndef DISABLE_GUI
    Component::State* state = c.active_scene->getComponent<Component::State>(eid);
    if (state == nullptr)
        return;
#endif
}
