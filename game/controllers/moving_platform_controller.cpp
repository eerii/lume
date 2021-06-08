//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "moving_platform_controller.h"

#include "s_collider.h"

#define EPSILON actor->max_move_speed * c.physics_delta

using namespace Verse;

bool Controller::MovingPlatform::controller(Config &c, EntityID eid, actor_move_func move) {
    Component::Collider* col = c.active_scene->getComponent<Component::Collider>(eid);
    Component::Actor* actor = c.active_scene->getComponent<Component::Actor>(eid);
    
    if (actor->patrol_points.size() > 0) {
        int next_patrol_point = (actor->current_patrol_point + 1) % actor->patrol_points.size();
        Vec2f difference = (actor->patrol_points[next_patrol_point] - col->transform.pos()).to_float();
        actor->vel = difference.normal() * actor->max_move_speed;
        if (difference.length() <= EPSILON) {
            actor->current_patrol_point = next_patrol_point;
        }
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
                Vec2f to_move = actor->remainder + actor->vel * c.physics_delta;
                Vec2 movement = Vec2(floor(to_move.x), floor(to_move.y));
                riding_actor->extra_vel = actor->vel;
            }
        }
    }
    col->transform += Vec2::j + offset;
    
    move(c, eid, false);
    
    return true;
}
