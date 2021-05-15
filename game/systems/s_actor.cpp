//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_actor.h"

#include "game.h"

#include "s_collider.h"

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
#ifdef TEXTURE
    Component::Texture* texture = scene.getComponent<Component::Texture>(eid);
#endif
    
    actor->controller();

    //Gravity
    if (actor->has_gravity && !actor->is_on_ground) {
        actor->vel.y += gravity * DELTA * c.game_speed;
    }
    
    //Terminal Velocity
    if (actor->vel.y > actor->max_fall_speed)
        actor->vel.y = actor->max_fall_speed;
    
    if (actor->vel != Vec2f(0,0)) {
        Vec2f total = actor->remainder + actor->vel * DELTA * c.game_speed;
        Vec2f to_move = Vec2f((int)total.x, (int)total.y);
        actor->remainder = total - to_move;
        
        //MoveX
        int sx = sign(to_move.x);
        while (to_move.x != 0) {
            collider->transform += Vec2::i * sx;
            
            bool isColliding = System::Collider::checkCollisions(eid, scene);
            if (isColliding) {
                collider->transform -= Vec2::i * sx;
                
                to_move.x = 0;
                actor->remainder.x = 0;
                actor->vel.x = 0;
            } else {
                to_move.x -= sx;
                
                //Check on ground
                if (to_move.y == 0) {
                    collider->transform += Vec2::j;
                    bool isColliding = System::Collider::checkCollisions(eid, scene);
                    if (!isColliding)
                        actor->is_on_ground = false;
                    collider->transform -= Vec2::j;
                }
            }
        }
        
        //MoveY
        int sy = sign(to_move.y);
        while (to_move.y != 0) {
            collider->transform += Vec2::j * sy;
            
            bool isColliding = System::Collider::checkCollisions(eid, scene);
            if (isColliding) {
                collider->transform -= Vec2::j * sy;
                
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
        
#ifdef TEXTURE
        texture->transform = collider->transform.pos();
#endif
#ifdef CAMERA
        Component::Camera* camera = scene.getComponent<Component::Camera>(eid);
        if (camera != nullptr)
            camera->target_pos = collider->transform.pos();
#endif
    }
#ifdef FIRE
        Component::Fire* fire = scene.getComponent<Component::Fire>(eid);
        if (fire != nullptr)
            fire->transform = collider->transform.pos() + fire->offset;
#endif
}
