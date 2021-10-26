//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "moving_platform_controller.h"

#include "game.h"
#include "s_collider.h"
#include "s_actor.h"

#define EPSILON 1

using namespace Verse;

bool Controller::MovingPlatform::controller(Config &c, EntityID eid) {
    Component::Collider* col = c.active_scene->getComponent<Component::Collider>(eid);
    Component::Actor* actor = c.active_scene->getComponent<Component::Actor>(eid);
    Component::Patrol* patrol = c.active_scene->getComponent<Component::Patrol>(eid);
    
    if (patrol->points.size() > 0) {
        int next_patrol_point = (patrol->current + 1) % patrol->points.size();
        Vec2<float> difference = (patrol->points[next_patrol_point] - col->transform.pos()).to<float>();
        actor->vel = difference.normal() * actor->max_move_speed;
        if (difference.length() <= EPSILON)
            patrol->current = next_patrol_point;
    }
    
    col->transform -= Vec2(0,1);
    System::Collider::CollisionInfo collisions = System::Collider::checkCollisions(c, eid);
    for (System::Collider::CollisionInfoPair collision : collisions) {
        if (collision.second[System::Collider::Layers::Actor]) {
            Component::Collider* riding_col = c.active_scene->getComponent<Component::Collider>(collision.first);
            Component::Actor* riding_actor = c.active_scene->getComponent<Component::Actor>(collision.first);
            
            bool above = riding_col->transform.y + riding_col->transform.h <= col->transform.y + 1;
            if (above) {
                riding_actor->extra_vel.x = actor->vel.x;
                if (riding_actor->vel.x == 0)
                    riding_actor->remainder.x = actor->remainder.x;
                
                riding_actor->extra_vel.y = actor->vel.y;
                riding_actor->remainder.y = actor->remainder.y;
            }
        }
    }
    col->transform += Vec2(0,1);
    
    System::Actor::move(c, eid);
    
    return true;
}
