//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "moving_platform_controller.h"

#include "s_collider.h"

#define EPSILON 1

using namespace Verse;

bool Controller::MovingPlatform::controller(Config &c, EntityID eid, actor_move_func move) {
    Component::Collider* col = c.active_scene->getComponent<Component::Collider>(eid);
    Component::Actor* actor = c.active_scene->getComponent<Component::Actor>(eid);
    Component::Patrol* patrol = c.active_scene->getComponent<Component::Patrol>(eid);
    
    if (patrol->points.size() > 0) {
        int next_patrol_point = (patrol->current + 1) % patrol->points.size();
        Vec2f difference = (patrol->points[next_patrol_point] - col->transform.pos()).to_float();
        actor->vel = difference.normal() * actor->max_move_speed;
        if (difference.length() <= EPSILON)
            patrol->current = next_patrol_point;
    }
    
    Vec2 offset = (actor->vel.y > 0) ? Vec2(0, floor(actor->vel.y * c.physics_delta)) : Vec2(0,0);
    col->transform -= Vec2::j + offset;
    System::Collider::CollisionInfo collisions = System::Collider::checkCollisions(c, eid);
    for (System::Collider::CollisionInfoPair collision : collisions) {
        if (collision.second[System::Collider::Layers::Actor]) {
            Component::Collider* riding_col = c.active_scene->getComponent<Component::Collider>(collision.first);
            Component::Actor* riding_actor = c.active_scene->getComponent<Component::Actor>(collision.first);
            
            float extra_padding_going_up = (actor->vel.y < 0) ? actor->vel.y * c.physics_delta : 0;
            bool above = riding_col->transform.y + riding_col->transform.h <= col->transform.y + 1 - extra_padding_going_up;
            if (above) {
                riding_actor->extra_vel = actor->vel;
            }
        }
    }
    col->transform += Vec2::j + offset;
    
    move(c, eid);
    
    return true;
}
