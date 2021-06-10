//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_actor.h"

#include "game.h"

#include "s_collider.h"
#include "s_scene_transition.h"
#include "s_fire.h"
#include "r_textures.h"

using namespace Verse;
using namespace State;

void System::Actor::update(Config &c) {
    for (EntityID e : SceneView<Component::Actor>(*c.active_scene)) {
        Component::Actor* actor = c.active_scene->getComponent<Component::Actor>(e);
        if (not actor->controller())
            return;
    }
}

bool System::Actor::move(Config &c, EntityID eid, State::StateType state) {
    Component::Actor* actor = c.active_scene->getComponent<Component::Actor>(eid);
    Component::Collider* collider = c.active_scene->getComponent<Component::Collider>(eid);
    
    PlayerStates* p_state = nullptr;
    if (std::holds_alternative<PlayerStates*>(state))
        p_state = std::get<PlayerStates*>(state);

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
        
        ui8 colliding = collisions(c, eid, state);
        
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
                
                ui8 colliding = collisions(c, eid, state);
                
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
        
        ui8 colliding = collisions(c, eid, state);
        
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
    texture->transform = collider->transform.pos();
    
    Component::Fire* fire = c.active_scene->getComponent<Component::Fire>(eid);
    if (fire != nullptr)
        fire->transform = collider->transform.pos() + fire->offset;
        
    return true;
}

ui8 System::Actor::collisions(Config &c, EntityID eid, State::StateType state, bool perform_actions) {
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
            
            Component::Fire* fire = c.active_scene->getComponent<Component::Fire>(collision.first);
            Component::Light* light = c.active_scene->getComponent<Component::Light>(collision.first);
            
            c.active_scene->checkpoints.push_back(c_col->transform.pos() - Vec2(0,4));
            
            fire = c.active_scene->addComponent<Component::Fire>(collision.first);
            Vec2 offset = Vec2(c_col->transform.w * 0.5f - 5.5, -c_col->transform.h);
            fire->transform = Rect2(c_col->transform.pos() + offset, Vec2(11, 11));
            fire->dir = Vec2::j;
            fire->fps = 3;
            fire->freq = 16;
            fire->octaves = 16;
            fire->seed = (rand() / RAND_MAX) % 1000;
            fire->layer = 0;
            Graphics::Texture::loadTexture("res/graphics/flame.png", fire->flame_tex);
            System::Fire::init(fire);
            
            light = c.active_scene->addComponent<Component::Light>(collision.first);
            light->pos = Vec2(c_col->transform.w * 0.5f, -c_col->transform.h * 0.5f);
            light->radius = 50;
            
            c.active_scene->removeComponent<Component::Collider>(collision.first);
        }
        
        if (collision.second[System::Collider::Layers::Platform]) {
            Component::Collider* platform_collider = c.active_scene->getComponent<Component::Collider>(collision.first);
            Component::Collider* actor_collider = c.active_scene->getComponent<Component::Collider>(eid);
            Component::Actor* actor = c.active_scene->getComponent<Component::Actor>(eid);
            
            bool above = actor_collider->transform.y + actor_collider->transform.h <= platform_collider->transform.y + 1;
            if (above and actor->vel.y > -1)
                solid = true;
        }
        
        if (collision.second[System::Collider::Layers::SolidPlatform])
            solid = true;
        
        if (collision.second[System::Collider::Layers::Ground])
            solid = true;
    }
    
    return solid ? Colliding::Solid : Colliding::Transparent;
}
