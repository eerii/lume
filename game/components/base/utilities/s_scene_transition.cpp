//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_scene_transition.h"

#include "serialization.h"
#include "log.h"

#include "s_tilemap.h"
#include "s_texture.h"
#include "player_controller.h"

#include "gui.h"
#include "gui_types.h"

using namespace Verse;

void System::SceneTransition::handle(Config &c, Component::SceneTransition* transition) {
    if (transition->to_scene == nullptr) {
        transition->to_scene = new Scene();
        Serialization::loadScene(transition->scene_name, transition->to_scene, c);
        log::warn("Scene loaded from the fail detector, to scene was a nullptr");
    }
    
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
        if (prev_actor != nullptr) {
            Component::Actor* new_actor = new_scene->getComponent<Component::Actor>(new_player);
            new_actor->vel = prev_actor->vel;
            new_actor->remainder = prev_actor->remainder;
        }
        
        //TODO: Pass player component
        
        prev_scene->removeEntity(prev_player);
        
        c.active_camera = new_scene->getComponent<Component::Camera>(new_player);
        if (c.active_camera == nullptr)
            log::error("Failed to get the active camera!");
        c.active_scene = new_scene;
        
        std::map<str, Scene*> transitions = {};
        for (EntityID e : SceneView<Component::SceneTransition>(*prev_scene)) {
            Component::SceneTransition* t = prev_scene->getComponent<Component::SceneTransition>(e);
            if (t->to_scene != nullptr) {
                transitions.insert(std::pair<str, Scene*>(t->scene_name, t->to_scene));
            }
        }
        
        std::vector<str> do_not_delete = {};
        for (EntityID e : SceneView<Component::SceneTransition>(*new_scene)) {
            Component::SceneTransition* t = new_scene->getComponent<Component::SceneTransition>(e);
            
            if (prev_scene == nullptr) {
                log::error("Previous scene is null pointer (scene transition)");
            }
            if (t->scene_name == prev_scene->name) {
                t->to_scene = prev_scene;
                continue;
            }
            if (transitions.find(t->scene_name) != transitions.end()) {
                t->to_scene = transitions[t->scene_name];
                do_not_delete.push_back(t->scene_name);
                continue;
            }
            
            if (t->to_scene == nullptr) {
                t->to_scene = new Scene();
                Serialization::loadScene(t->scene_name, t->to_scene, c);
                log::debug("Loaded scene " + t->to_scene->name);
            }
        }
        
        for (EntityID e : SceneView<Component::SceneTransition>(*prev_scene)) {
            Component::SceneTransition* t = prev_scene->getComponent<Component::SceneTransition>(e);
            
            bool is_new = std::find(do_not_delete.begin(), do_not_delete.end(), t->scene_name) != do_not_delete.end();
            if (t->to_scene != nullptr and t->to_scene != new_scene and not is_new) {
                log::debug("Unloaded scene " + t->to_scene->name);
                delete t->to_scene;
                t->to_scene = nullptr;
            }
        }
        
        Controller::Player::resetState(c, new_player);
        System::Texture::clean();
        System::Tilemap::init(c);
    }
}

void System::SceneTransition::load(EntityID eid, YAML::Node &entity, Scene *s, Config &c) {
    Component::SceneTransition* transition = s->addComponent<Component::SceneTransition>(eid);
    if (not entity["scene_transition"]["scene"]) {
        log::error("You created a scene transition component for " + s->getName(eid) + " but it has no scene url");
        s->removeEntity(eid);
        return;
    }
    transition->scene_name = entity["scene_transition"]["scene"].as<str>();
    transition->to_pos =  entity["scene_transition"]["pos"] ? entity["scene_transition"]["pos"].as<Vec2>() : Vec2(0,0);
}

void System::SceneTransition::gui(Config &c, EntityID eid) {
#ifndef DISABLE_GUI
    
#endif
}