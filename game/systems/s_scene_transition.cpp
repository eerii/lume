//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_scene_transition.h"
#include "s_tilemap.h"
#include "serialization.h"
#include "log.h"

using namespace Verse;

void System::SceneTransition::handle(Config &c, Component::SceneTransition* transition) {
    handle(c, transition->to_scene, transition->to_pos);
}

void System::SceneTransition::handle(Config &c, Scene* new_scene, Vec2 new_pos) {
    Scene* prev_scene = c.active_scene;
    
    if (prev_scene != new_scene) {
        EntityID prev_player = 0;
        for (EntityID e : SceneView<Component::Player>(*prev_scene)) {
            prev_player = e;
        }
        
        EntityID new_player = Serialization::loadPlayer(new_scene, c);
        
        Component::Collider* new_col = new_scene->getComponent<Component::Collider>(new_player);
        new_col->transform = new_pos;
        
        Component::Actor* prev_actor = prev_scene->getComponent<Component::Actor>(prev_player);
        Component::Actor* new_actor = new_scene->getComponent<Component::Actor>(new_player);
        new_actor->vel = prev_actor->vel;
        new_actor->remainder = prev_actor->remainder;
        
        //TODO: Pass player component
        
        prev_scene->removeEntity(prev_player);
        
        c.active_camera = new_scene->getComponent<Component::Camera>(new_player);
        if (c.active_camera == nullptr)
            log::error("Failed to get the active camera!");
        c.active_scene = new_scene;
        
        for (EntityID e : SceneView<Component::SceneTransition>(*prev_scene)) {
            Component::SceneTransition* transition = prev_scene->getComponent<Component::SceneTransition>(e);
            if (transition->to_scene == new_scene)
                continue;
            delete transition->to_scene;
        }
        
        System::Tilemap::init(c);
    }
}
