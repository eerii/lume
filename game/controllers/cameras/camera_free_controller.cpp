//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "camera_free_controller.h"

#include <glm/ext.hpp>

#include "input.h"

#define CAM_MOVE 200

using namespace Verse;

bool Controller::Camera::Free::controller(Config &c, EntityID eid) {
    Component::Camera* cam = c.active_scene->getComponent<Component::Camera>(eid);
    
    if (Input::down(Input::Key::A))
        cam->target_pos.x = cam->pos.x - CAM_MOVE * c.physics_delta;
    if (Input::down(Input::Key::D))
        cam->target_pos.x = cam->pos.x + CAM_MOVE * c.physics_delta;
    if (Input::down(Input::Key::W))
        cam->target_pos.y = cam->pos.y - CAM_MOVE * c.physics_delta;
    if (Input::down(Input::Key::S))
        cam->target_pos.y = cam->pos.y + CAM_MOVE * c.physics_delta;
    
    /*if (Input::pressed(Input::Key::O))
        cam->scale += 0.25f;
    if (Input::pressed(Input::Key::U) and cam->scale > 0.25f)
        cam->scale -= 0.25f;*/
    
    return true;
}
