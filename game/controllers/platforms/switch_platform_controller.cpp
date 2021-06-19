//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "switch_platform_controller.h"

#include "s_collider.h"
#include <map>

using namespace Verse;

namespace {
    std::map<EntityID, ui8> platform_layer;
}

bool Controller::SwitchPlatform::controller(Config &c, EntityID eid, actor_move_func move) {
    Component::Collider* col = c.active_scene->getComponent<Component::Collider>(eid);
    Component::Texture* tex = c.active_scene->getComponent<Component::Texture>(eid);
    Component::Timer* timer = c.active_scene->getComponent<Component::Timer>(eid);
    
    if (timer->tid == 0)
        timer->tid = setTimer(timer->ms);
    
    if (checkTimer(timer->tid)) {
        timer->tid = setTimer(timer->ms);
        
        if (col->layer != System::Collider::Layers::Disabled) {
            platform_layer[eid] = col->layer;
            col->layer = System::Collider::Layers::Disabled;
            
            tex->transform.size = Vec2::zero; //TODO: Unactive texture animation?
        } else {
            if (platform_layer.find(eid) != platform_layer.end())
                col->layer = platform_layer[eid];
            else
                col->layer = System::Collider::Layers::Platform;
            platform_layer.erase(eid);
            
            tex->transform.size = col->transform.size;
        }
    }
    
    return true;
}
