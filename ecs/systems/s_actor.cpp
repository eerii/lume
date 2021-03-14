//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "game.h"
#include "s_actor.h"
#include "s_collider.h"
#include "s_camera.h"

using namespace Verse;

namespace {
    float gravity = 800;
}

void System::Actor::update(Config &c, Scene &scene) {
    for (EntityID e : SceneView<Component::Actor>(scene)) {
        System::Actor::move(c, scene, e);
    }
}

void System::Actor::move(Config &c, Scene &scene, EntityID eid) {
    Component::Actor* actor = scene.getComponent<Component::Actor>(eid);
    Component::Collider* collider = scene.getComponent<Component::Collider>(eid);
    Component::Texture* texture = scene.getComponent<Component::Texture>(eid);
    
    actor->controller();

    //Gravity
    if (actor->has_gravity && !actor->is_on_ground) {
        actor->vel.y += gravity * DELTA;
    }
    
    //Terminal Velocity
    if (actor->vel.y > actor->max_fall_speed)
        actor->vel.y = actor->max_fall_speed;
    
    if (actor->vel != Vec2(0,0)) {
        Vec2 total = actor->remainder + actor->vel * DELTA;
        Vec2 to_move = Vec2((int)total.x, (int)total.y);
        actor->remainder = total - to_move;
        
        //MoveX
        int sx = sign(to_move.x);
        if (to_move.x != 0)
            texture->is_reversed = sx == -1;
        while (to_move.x != 0) {
            collider->transform.pos += Vec2::i * sx;
            
            bool isColliding = System::Collider::checkCollisions(eid, scene);
            if (isColliding) {
                collider->transform.pos -= Vec2::i * sx;
                
                to_move.x = 0;
                actor->remainder.x = 0;
                actor->vel.x = 0;
            } else {
                to_move.x -= sx;
                
                //Check on ground
                if (to_move.y == 0) {
                    collider->transform.pos += Vec2::j;
                    bool isColliding = System::Collider::checkCollisions(eid, scene);
                    if (!isColliding)
                        actor->is_on_ground = false;
                    collider->transform.pos -= Vec2::j;
                }
            }
        }
        
        //MoveY
        int sy = sign(to_move.y);
        while (to_move.y != 0) {
            collider->transform.pos += Vec2::j * sy;
            
            bool isColliding = System::Collider::checkCollisions(eid, scene);
            if (isColliding) {
                collider->transform.pos -= Vec2::j * sy;
                
                to_move.y = 0;
                actor->remainder.y = 0;
                actor->vel.y = 0;
                
                if (sy > 0)
                    actor->is_on_ground = true;
            } else {
                to_move.y -= sy;
                
                actor->is_on_ground = false;
            }
        }
        
        texture->transform.pos = collider->transform.pos;
    }
    
    //Camera
    if (scene.getComponent<Component::Camera>(eid) != nullptr)
        System::Camera::move(c, collider->transform.pos + collider->transform.size / 2.0f);
}
