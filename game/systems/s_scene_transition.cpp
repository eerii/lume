//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_scene_transition.h"
#include "s_tilemap.h"
#include "log.h"

using namespace Verse;

void System::SceneTransition::handle(Config &c, Component::SceneTransition* transition) {
    EntityID prev_player = 0;
    for (EntityID e : SceneView<Component::Player>(*c.active_scene)) {
        prev_player = e;
    }
    
    EntityID player = 0;
    for (EntityID e : SceneView<Component::Player>(*transition->to_scene)) {
        player = e;
    }
    
    Component::Collider* new_col = transition->to_scene->getComponent<Component::Collider>(player);
    new_col->transform = transition->to_pos;
    
    Component::Actor* prev_actor = c.active_scene->getComponent<Component::Actor>(prev_player);
    Component::Actor* new_actor = transition->to_scene->getComponent<Component::Actor>(player);
    new_actor->vel = prev_actor->vel;
    new_actor->remainder = prev_actor->remainder;
    
    c.active_camera = transition->to_scene->getComponent<Component::Camera>(player);
    if (c.active_camera == nullptr)
        log::error("Failed to get the active camera!");
    c.active_scene = transition->to_scene;
    
    System::Tilemap::init(c);
}
