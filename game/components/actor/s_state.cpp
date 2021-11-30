//project fresa, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#include "s_state.h"

#include "gui.h"
#include "gui_types.h"
#include "state_machines_list.h"

using namespace Fresa;
void System::State::load(EntityID eid, YAML::Node &entity, Scene *s, Config &c) {
    //(lume)
    Component::State* state = s->addComponent<Component::State>(eid);
    if (entity["state"]["checkpoint"]) {
        state->states.push_back(::Fresa::State::Checkpoint::CheckpointSM());
        state->has_state["checkpoint"] = true;
        state->index["checkpoint"] = state->states.size() - 1;
    }
    //---
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
