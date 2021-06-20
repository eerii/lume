//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "falling_platform_controller.h"

#include "s_collider.h"
#include "s_actor.h"

using namespace Verse;

bool Controller::FallingPlatform::controller(Config &c, EntityID eid) {
    Component::Collider* col = c.active_scene->getComponent<Component::Collider>(eid);
    Component::Actor* actor = c.active_scene->getComponent<Component::Actor>(eid);
    Component::Timer* timer = c.active_scene->getComponent<Component::Timer>(eid);
    
    System::Actor::move(c, eid);
    
    bool above = checkActorAbove(c, eid);
    
    if (above and timer->tid == 0) {
        timer->tid = setTimer(timer->ms);
    }
    
    if (timer->tid != 0 and checkTimer(timer->tid)) {
        timer->tid = 0;
        actor->has_gravity = true;
    }
    
    if (actor->has_gravity and actor->vel.y == 0 and checkGroundBelow(c, eid)) {
        c.active_scene->removeComponent<Component::Timer>(eid);
        c.active_scene->removeComponent<Component::Actor>(eid);
    }
    
    if (*col->transform.y > 500)
        c.active_scene->removeEntity(eid);
    
    return true;
}

bool Controller::FallingPlatform::checkActorAbove(Config &c, EntityID eid) {
    Component::Collider* col = c.active_scene->getComponent<Component::Collider>(eid);
    
    col->transform -= Vec2::j;
    System::Collider::CollisionInfo collisions = System::Collider::checkCollisions(c, eid);
    col->transform += Vec2::j;
    
    for (System::Collider::CollisionInfoPair collision : collisions) {
        if (collision.second[System::Collider::Layers::Actor])
            return true;
    }
    return false;
}

bool Controller::FallingPlatform::checkGroundBelow(Config &c, EntityID eid) {
    Component::Collider* col = c.active_scene->getComponent<Component::Collider>(eid);
    
    col->transform += Vec2::j;
    System::Collider::CollisionInfo collisions = System::Collider::checkCollisions(c, eid);
    col->transform -= Vec2::j;
    
    for (System::Collider::CollisionInfoPair collision : collisions) {
        if (collision.second[System::Collider::Layers::Ground])
            return true;
    }
    return false;
}
