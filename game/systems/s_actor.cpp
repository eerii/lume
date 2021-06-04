//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_actor.h"

#include "game.h"

#include "s_collider.h"

using namespace Verse;
using namespace State;

namespace {
    std::bitset<MAX_COLLISION_LAYERS> collision_layers;
}

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
    
    Vec2f total = actor->remainder + actor->vel * c.physics_delta;
    Vec2 to_move = Vec2(floor(total.x), floor(total.y));
    actor->remainder = total - to_move.to_float();
    
    //MoveX
    int sx = sign(to_move.x);
    while (to_move.x != 0) {
        collider->transform += Vec2::i * sx;
        
        collision_layers = System::Collider::checkCollisions(c, eid);
        
        if (collision_layers[Component::ColliderLayers::SCENE])
            return false;
        
        if (collision_layers[Component::ColliderLayers::WATER]) {
            actor->damage();
            return false;
        }
        
        if (collision_layers.any()) {
            collider->transform -= Vec2::i * sx;
            
            to_move.x = 0;
            actor->remainder.x = 0;
            actor->vel.x = 0;
        } else {
            to_move.x -= sx;
            
            //Check on ground
            if (to_move.y == 0) {
                collider->transform += Vec2::j;
                
                collision_layers = System::Collider::checkCollisions(c, eid);
                
                if (collision_layers[Component::ColliderLayers::SCENE])
                    return false;
                
                if (collision_layers[Component::ColliderLayers::WATER]) {
                    actor->damage();
                    return false;
                }
                
                if (collision_layers.none() and p_state != nullptr)
                    p_state->jump.handle(Player::FallEvent());
                
                collider->transform -= Vec2::j;
            }
        }
    }
    
    //MoveY
    int sy = sign(to_move.y);
    while (to_move.y != 0) {
        collider->transform += Vec2::j * sy;
        
        collision_layers = System::Collider::checkCollisions(c, eid);
        
        if (collision_layers[Component::ColliderLayers::SCENE])
            return false;
        
        if (collision_layers[Component::ColliderLayers::WATER]) {
            actor->damage();
            return false;
        }
        
        if (collision_layers.any()) {
            collider->transform -= Vec2::j * sy;
            
            to_move.y = 0;
            actor->remainder.y = 0;
            actor->vel.y = 0;
            
            if (sy > 0 and p_state != nullptr)
                p_state->jump.handle(Player::TouchGroundEvent());
        } else {
            to_move.y -= sy;
            
            p_state->jump.handle(Player::FallEvent());
        }
    }
    
#ifdef TEXTURE
    Component::Texture* texture = c.active_scene->getComponent<Component::Texture>(eid);
    texture->transform = collider->transform.pos();
#endif
#ifdef FIRE
    Component::Fire* fire = c.active_scene->getComponent<Component::Fire>(eid);
    if (fire != nullptr)
        fire->transform = collider->transform.pos() + fire->offset;
#endif
        
    return true;
}
