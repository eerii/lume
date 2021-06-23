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

bool Controller::SwitchPlatform::controller(Config &c, EntityID eid) {
    Component::Collider* col = c.active_scene->getComponent<Component::Collider>(eid);
    Component::Animation* anim = c.active_scene->getComponent<Component::Animation>(eid);
    Component::Timer* timer = c.active_scene->getComponent<Component::Timer>(eid);
    
    if (timer->tid[0] == 0)
        timer->tid[0] = setTimer(timer->ms[0]);
    
    if (checkTimer(timer->tid[0])) {
        timer->tid[0] = setTimer(timer->ms[0]);
        
        if (col->layer != System::Collider::Layers::Disabled) {
            platform_layer[eid] = col->layer;
            col->layer = System::Collider::Layers::Disabled;
            
            anim->target_key = "inactive";
        } else {
            if (platform_layer.find(eid) != platform_layer.end())
                col->layer = platform_layer[eid];
            else
                col->layer = System::Collider::Layers::Platform;
            platform_layer.erase(eid);
            
            anim->queue.push_back("pre_active_1");
            anim->queue.push_back("pre_active_2");
            anim->queue.push_back("active");
        }
    } else if (col->layer != System::Collider::Layers::Disabled) {
        ui32 t = getTimerRemainder(timer->tid[0]);
        if (t < (ui32)(0.2f * (float)timer->ms[0]))
            anim->target_key = "pre_inactive";
    }
    
    return true;
}
