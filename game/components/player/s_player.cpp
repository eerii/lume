//project fresa, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#include "s_player.h"

#include "gui.h"
#include "gui_types.h"

using namespace Fresa;

void System::Player::load(EntityID eid, YAML::Node &entity, Scene *s, Config &c) {
    
}

void System::Player::gui(Config &c, EntityID eid) {
#ifndef DISABLE_GUI
    Component::Player* player = c.active_scene->getComponent<Component::Player>(eid);
    if (player == nullptr)
        return;
    
    Gui::draw_float(player->light_strength, "light strenght", eid);
#endif
}
