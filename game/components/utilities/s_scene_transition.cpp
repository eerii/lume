//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_scene_transition.h"

#include "serialization.h"
#include "log.h"

#include "s_tilemap.h"
#include "s_texture.h"

#include "component_list.h"
#include "scene_list.h"

#include "gui.h"
#include "gui_types.h"

#include "player_controller.h"

using namespace Verse;

void System::SceneTransition::handle(Config &c, Component::SceneTransition* transition) {
    if (transition->to_scene == nullptr) {
        transition->to_scene = new Scene();
        Serialization::loadScene(transition->scene_name, transition->to_scene, c);
        log::warn("Scene loaded from the fail detector, to scene was a nullptr");
    }
    
    handle(c, transition->to_scene, transition->to_pos);
}

void System::SceneTransition::handle(Config &c, Scene* new_scene, Vec2<int> new_pos) {
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
        
        Component::Player* prev_player_c = prev_scene->getComponent<Component::Player>(prev_player);
        if (prev_player_c != nullptr) {
            Component::Player* new_player_c = new_scene->getComponent<Component::Player>(new_player);
            new_player_c->light_strength = prev_player_c->light_strength;
        }
        
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
    transition->to_pos =  entity["scene_transition"]["pos"] ? entity["scene_transition"]["pos"].as<Vec2<int>>() : Vec2(0,0);
}

//TODO: Scene transition save

void System::SceneTransition::gui(Config &c, EntityID eid) {
#ifndef DISABLE_GUI
    Component::SceneTransition* trans = c.active_scene->getComponent<Component::SceneTransition>(eid);
    if (trans == nullptr)
        return;
    
    int scene_index = 0;
    auto it = std::find(scenes.begin(), scenes.end(), trans->scene_name);
    if (it != scenes.end())
        scene_index = (int)std::distance(scenes.begin(), it);
    
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("new scene");
    
    ImGui::TableSetColumnIndex(1);
    str layer_label = "##scenetrans" + std::to_string(eid);
    ImGui::SetNextItemWidth(ImGui::GetColumnWidth());
    if (ImGui::Combo(layer_label.c_str(), &scene_index, scenes) and trans->scene_name != scenes[scene_index]) {
        trans->scene_name = scenes[scene_index];
        trans->to_scene = new Scene();
        Serialization::loadScene(trans->scene_name, trans->to_scene, c);
    }
    ImGui::TableNextRow();
    
    Verse::Gui::draw_vec2(trans->to_pos.x, trans->to_pos.y, "new scene pos", eid);
#endif
}
