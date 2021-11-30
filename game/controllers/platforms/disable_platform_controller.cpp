//project fresa, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#include "disable_platform_controller.h"

#include "s_collider.h"

using namespace Fresa;

namespace {
    std::map<EntityID, ui8> platform_layer;
}

bool Controller::DisablePlatform::controller(Config &c, EntityID eid) {
    Component::Collider* col = c.active_scene->getComponent<Component::Collider>(eid);
    Component::Animation* anim = c.active_scene->getComponent<Component::Animation>(eid);
    Component::Timer* timer = c.active_scene->getComponent<Component::Timer>(eid);
    
    if (col->layer != System::Collider::Layers::Disabled and timer->tid[0] == 0) {
        col->transform -= Vec2(0,1);
        System::Collider::CollisionInfo collisions = System::Collider::checkCollisions(c, eid);
        for (System::Collider::CollisionInfoPair collision : collisions) {
            if (collision.second[System::Collider::Layers::Actor]) {
                Component::Collider* riding_col = c.active_scene->getComponent<Component::Collider>(collision.first);
                Component::Actor* riding_actor = c.active_scene->getComponent<Component::Actor>(collision.first);
                
                bool above = riding_col->transform.y + riding_col->transform.h <= col->transform.y + 1;
                if (above and riding_actor->vel.y >= 0) {
                    anim->queue.push_back("pre_inactive");
                    timer->tid[0] = setTimer(timer->ms[0]);
                }
            }
        }
        col->transform += Vec2(0,1);
    }
    
    if (checkTimer(timer->tid[0])) {
        timer->tid[0] = 0;
        
        anim->target_key = "inactive";
        
        platform_layer[eid] = col->layer;
        col->layer = System::Collider::Layers::Disabled;
        
        timer->tid[1] = setTimer(timer->ms[1]);
    }
    
    if (checkTimer(timer->tid[1])) {
        timer->tid[1] = 0;
        
        anim->queue.push_back("pre_active_1");
        anim->queue.push_back("pre_active_2");
        anim->queue.push_back("active");
        
        if (platform_layer.find(eid) != platform_layer.end())
            col->layer = platform_layer[eid];
        else
            col->layer = System::Collider::Layers::Platform;
        platform_layer.erase(eid);
    }
    
    return true;
}
