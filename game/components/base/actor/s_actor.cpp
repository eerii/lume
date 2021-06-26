//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_actor.h"

#include "game.h"

#include "s_collider.h"
#include "s_scene_transition.h"
#include "r_textures.h"
#include "controller_list.h"

#include "gui.h"
#include "gui_types.h"

using namespace Verse;
using namespace State;

void System::Actor::update(Config &c) {
    for (EntityID e : SceneView<Component::Actor>(*c.active_scene)) {
        Component::Actor* actor = c.active_scene->getComponent<Component::Actor>(e);
        if (not actor->controller())
            return;
    }
}

bool System::Actor::move(Config &c, EntityID eid) {
    Component::Actor* actor = c.active_scene->getComponent<Component::Actor>(eid);
    Component::Collider* collider = c.active_scene->getComponent<Component::Collider>(eid);
    
    PlayerStates* p_state = nullptr;
    Component::State* state = c.active_scene->getComponent<Component::State>(eid);
    if (state != nullptr and std::holds_alternative<PlayerStates*>(state->state))
        p_state = std::get<PlayerStates*>(state->state);

    //Gravity
    bool grounded = (p_state != nullptr) ?
                     p_state->jump.is(Player::GroundedState()) or
                     p_state->jump.is(Player::CrouchingState()) : false;
    if (actor->has_gravity and not grounded) {
        actor->vel += c.gravity_dir * c.gravity * c.physics_delta;
    }
    
    //Terminal Velocity
    if (actor->vel.y > actor->max_fall_speed)
        actor->vel.y = actor->max_fall_speed;
    
    Vec2f total = actor->remainder + (actor->vel + actor->extra_vel) * c.physics_delta;
    Vec2 to_move = Vec2(floor(total.x), floor(total.y));
    actor->remainder = total - to_move.to_float();
    
    //MoveX
    int sx = sign(to_move.x);
    while (to_move.x != 0) {
        collider->transform += Vec2::i * sx;
        
        ui8 colliding = collisions(c, eid);
        
        if (colliding == Colliding::Exit)
            return false;
        
        if (colliding == Colliding::Solid) {
            collider->transform -= Vec2::i * sx;
            
            to_move.x = 0;
            actor->remainder.x = 0;
            actor->vel.x = 0;
        }
        
        if (colliding == Colliding::Transparent) {
            to_move.x -= sx;
            
            //Check on ground
            if (to_move.y == 0) {
                collider->transform += Vec2::j;
                
                ui8 colliding = collisions(c, eid);
                
                if (colliding == Colliding::Exit)
                    return false;
                
                if (colliding != Colliding::Solid and p_state != nullptr)
                    p_state->jump.handle(Player::FallEvent());
                
                collider->transform -= Vec2::j;
            }
        }
    }
    
    //MoveY
    int sy = sign(to_move.y);
    while (to_move.y != 0) {
        collider->transform += Vec2::j * sy;
        
        ui8 colliding = collisions(c, eid);
        
        if (colliding == Colliding::Exit)
            return false;
        
        if (colliding == Colliding::Solid) {
            collider->transform -= Vec2::j * sy;
            
            to_move.y = 0;
            actor->remainder.y = 0;
            actor->vel.y = 0;
            
            if (sy > 0 and p_state != nullptr)
                p_state->jump.handle(Player::TouchGroundEvent());
        }
        
        if (colliding == Colliding::Transparent) {
            to_move.y -= sy;
            
            if (p_state != nullptr)
                p_state->jump.handle(Player::FallEvent());
        }
    }
    
    Component::Texture* texture = c.active_scene->getComponent<Component::Texture>(eid);
    if (texture != nullptr)
        texture->render_pos = collider->transform.pos;
        
    return true;
}

ui8 System::Actor::collisions(Config &c, EntityID eid, bool perform_actions) {
    System::Collider::CollisionInfo collision_info = System::Collider::checkCollisions(c, eid);
    
    bool solid = false;
    
    for (System::Collider::CollisionInfoPair collision : collision_info) {
        
        if (collision.second[System::Collider::Layers::Scene]) {
            Component::SceneTransition* c_transition = c.active_scene->getComponent<Component::SceneTransition>(collision.first);
            if (c_transition != nullptr) {
                if (perform_actions)
                    System::SceneTransition::handle(c, c_transition);
                return Colliding::Exit;
            }
        }
        
        if (collision.second[System::Collider::Layers::Water]) {
            Component::Actor* actor = c.active_scene->getComponent<Component::Actor>(eid);
            if (perform_actions)
                actor->damage();
            return Colliding::Exit;
        }
        
        if (collision.second[System::Collider::Layers::Checkpoint] and perform_actions) {
            Component::Collider* c_col = c.active_scene->getComponent<Component::Collider>(collision.first);
            
            c.active_scene->checkpoints.push_back(c_col->transform.pos);
            
            Component::Light* light = c.active_scene->addComponent<Component::Light>(collision.first);
            light->pos = Vec2(*c_col->transform.w * 0.5f, -*c_col->transform.h * 0.5f);
            light->radius = 50;
            
            Component::Animation* anim = c.active_scene->getComponent<Component::Animation>(collision.first);
            if (anim != nullptr)
                anim->target_key = "active";
            
            Component::Noise* noise = c.active_scene->getComponent<Component::Noise>(collision.first);
            if (noise != nullptr)
                noise->enabled = true;
            
            c.active_scene->removeComponent<Component::Collider>(collision.first);
        }
        
        if (collision.second[System::Collider::Layers::Platform]) {
            Component::Collider* platform_collider = c.active_scene->getComponent<Component::Collider>(collision.first);
            Component::Collider* actor_collider = c.active_scene->getComponent<Component::Collider>(eid);
            Component::Actor* actor = c.active_scene->getComponent<Component::Actor>(eid);
            
            bool falling = false;
            Component::State* state = c.active_scene->getComponent<Component::State>(eid);
            if (state != nullptr and std::holds_alternative<PlayerStates*>(state->state))
                falling = std::get<PlayerStates*>(state->state)->jump.is(Player::FallingFromPlatformState());
            
            bool above = *actor_collider->transform.y + *actor_collider->transform.h <= *platform_collider->transform.y + 1;
            if (not falling and above and actor->vel.y >= 0) {
                solid = true;
            }
        }
        
        if (collision.second[System::Collider::Layers::SolidPlatform])
            solid = true;
        
        if (collision.second[System::Collider::Layers::Ground])
            solid = true;
    }
    
    return solid ? Colliding::Solid : Colliding::Transparent;
}

void System::Actor::load(EntityID eid, YAML::Node &entity, Scene *s, Config &c) {
    Component::Actor* actor = s->addComponent<Component::Actor>(eid);
    if (entity["actor"]["controller"]) {
        str name = entity["actor"]["controller"].as<str>();
        if (controllers.find(name) != controllers.end())
            actor->controller = [&c, eid, name](){ return controllers[name](c, eid); };
        else
            log::error("Error getting controller for " + c.active_scene->getName(eid));
        
        if (damage_controllers.find(name) != damage_controllers.end())
            actor->damage = [&c, name](){ return damage_controllers[name](c); };
        
        actor->current_controller = name;
    }
    if (entity["actor"]["max_move_speed"])
        actor->max_move_speed = entity["actor"]["max_move_speed"].as<int>();
    if (entity["actor"]["max_fall_speed"])
        actor->max_fall_speed = entity["actor"]["max_fall_speed"].as<int>();
    if (entity["actor"]["max_move_speed"] and not entity["actor"]["max_fall_speed"])
        actor->max_fall_speed = entity["actor"]["max_move_speed"].as<int>();
    if (entity["actor"]["acc_ground"])
        actor->acc_ground = entity["actor"]["acc_ground"].as<int>();
    if (entity["actor"]["friction_ground"])
        actor->friction_ground = entity["actor"]["friction_ground"].as<int>();
    if (entity["actor"]["friction_air"])
        actor->friction_air = entity["actor"]["friction_air"].as<int>();
    if (entity["actor"]["friction_extra"])
        actor->friction_extra = entity["actor"]["friction_extra"].as<int>();
    if (entity["actor"]["friction_air"] and not entity["actor"]["friction_extra"])
        actor->friction_extra = entity["actor"]["friction_air"].as<int>();
    if (entity["actor"]["has_gravity"])
        actor->has_gravity = entity["actor"]["has_gravity"].as<bool>();
    if (entity["actor"]["collision_mask"]) {
        if (entity["actor"]["collision_mask"].IsScalar()) {
            auto it = std::find(System::Collider::layers_name.begin(), System::Collider::layers_name.end(),
                                entity["actor"]["collision_mask"].as<str>());
            if (it != System::Collider::layers_name.end())
                actor->collision_mask.set(std::distance(System::Collider::layers_name.begin(), it));
        } else {
            for (str m : entity["actor"]["collision_mask"].as<std::vector<str>>()) {
                auto it = std::find(System::Collider::layers_name.begin(), System::Collider::layers_name.end(), m);
                if (it != System::Collider::layers_name.end())
                    actor->collision_mask.set(std::distance(System::Collider::layers_name.begin(), it));
            }
        }
    }
}

void System::Actor::save(Component::Actor *actor, str path, std::vector<str> &key) {
    key[2] = "actor";
    
    key[3] = "controller";
    Serialization::appendYAML(path, key, (str)actor->current_controller, true);
    
    if (actor->max_move_speed != 0) {
        key[3] = "max_move_speed";
        Serialization::appendYAML(path, key, actor->max_move_speed, true);
    }
    
    if (actor->max_fall_speed != 0) {
        key[3] = "max_fall_speed";
        Serialization::appendYAML(path, key, actor->max_fall_speed, true);
    }
    
    if (actor->acc_ground != 0) {
        key[3] = "acc_ground";
        Serialization::appendYAML(path, key, actor->acc_ground, true);
    }
    
    if (actor->friction_ground != 0) {
        key[3] = "friction_ground";
        Serialization::appendYAML(path, key, actor->friction_ground, true);
    }
    
    if (actor->friction_ground != 0) {
        key[3] = "friction_air";
        Serialization::appendYAML(path, key, actor->friction_ground, true);
    }
    
    if (actor->friction_extra != 0) {
        key[3] = "friction_extra";
        Serialization::appendYAML(path, key, actor->friction_extra, true);
    }
    
    key[3] = "has_gravity";
    Serialization::appendYAML(path, key, (bool)actor->has_gravity, true);
    
    //Collision Mask
    if (actor->collision_mask.any()) {
        key[3] = "collision_mask";
        std::vector<str> collision_mask_names;
        for (int i = 0; i < MAX_COLLISION_LAYERS; i++) {
            if (actor->collision_mask[i])
                collision_mask_names.push_back(component_names[i]);
        }
        Serialization::appendYAML(path, key, collision_mask_names, true);
    }
}

void System::Actor::gui(Config &c, EntityID eid) {
#ifndef DISABLE_GUI
    Component::Actor* actor = c.active_scene->getComponent<Component::Actor>(eid);
    if (actor == nullptr)
        return;
    
    Verse::Gui::draw_vec2(actor->vel.x, actor->vel.y, "vel", eid);
    ImGui::TableNextRow();
    Verse::Gui::draw_vec2(actor->remainder.x, actor->remainder.y, "remainder", eid);
    ImGui::TableNextRow();
    
    Verse::Gui::draw_int(actor->max_move_speed, "max move speed", eid);
    ImGui::TableNextRow();
    Verse::Gui::draw_int(actor->max_fall_speed, "max fall speed", eid);
    ImGui::TableNextRow();
    
    Verse::Gui::draw_int(actor->acc_ground, "acc (ground)", eid);
    ImGui::TableNextRow();
    Verse::Gui::draw_int(actor->friction_ground, "friction (ground)", eid);
    ImGui::TableNextRow();
    Verse::Gui::draw_int(actor->friction_air, "friction (air)", eid);
    ImGui::TableNextRow();
    Verse::Gui::draw_int(actor->friction_extra, "friction (extra)", eid);
    ImGui::TableNextRow();
    
    Verse::Gui::draw_bool(actor->has_gravity, "gravity", eid);
    ImGui::TableNextRow();
    
    
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("controller");
    
    ImGui::TableSetColumnIndex(1);
    str controller_label = "##controller" + std::to_string(eid);
    ImGui::SetNextItemWidth(ImGui::GetColumnWidth());
    
    std::vector<str> available_controllers;
    ui8 current_controller_index = 0;
    ui8 i = 0;
    for (std::pair<str, std::function<bool(Config &c, EntityID eid)>> ct : controllers) {
        available_controllers.push_back(ct.first);
        if (actor->current_controller == ct.first)
            current_controller_index = i;
        i++;
    }
    
    ui8 prev_controller_index = current_controller_index;
    ImGui::Combo(controller_label.c_str(), current_controller_index, available_controllers);
    
    if (prev_controller_index != current_controller_index) {
        str name = available_controllers[current_controller_index];
        actor->controller = [&c, eid, name](){
            return controllers[name](c, eid);
        };
        if (damage_controllers.find(name) != damage_controllers.end()) {
            actor->damage = [&c, name](){
                return damage_controllers[name](c);
            };
        }
        actor->current_controller = name;
    }
    
    ImGui::TableNextRow();
    
    
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("collision mask");
    ImGui::TableSetColumnIndex(1);
    i = 0;
    for (str layer : System::Collider::layers_name) {
        str layer_label = layer + "##" + std::to_string(eid);
        bool layer_active = actor->collision_mask[i];
        
        ImGui::SetNextItemWidth(ImGui::GetColumnWidth());
        if (ImGui::Selectable(layer_label.c_str(), layer_active))
            (actor->collision_mask[i]) ? actor->collision_mask.reset(i) : actor->collision_mask.set(i);
        i++;
    }
#endif
}
