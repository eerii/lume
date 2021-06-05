//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "camera_free_controller.h"

#include <glm/ext.hpp>

#include "input.h"

#define CAM_SPEED 200

using namespace Verse;

bool Controller::Camera::Free::controller(Config &c, EntityID eid) {
    Component::Camera* cam = c.active_scene->getComponent<Component::Camera>(eid);
    
    if (Input::down(Input::Key::A))
        cam->vel.x = -CAM_SPEED;
    if (Input::down(Input::Key::D))
        cam->vel.x = CAM_SPEED;
    if (Input::down(Input::Key::W))
        cam->vel.y = -CAM_SPEED;
    if (Input::down(Input::Key::S))
        cam->vel.y = CAM_SPEED;
    
    if (not Input::down(Input::Key::A) and not Input::down(Input::Key::D))
        cam->vel.x = 0;
    if (not Input::down(Input::Key::W) and not Input::down(Input::Key::S))
        cam->vel.y = 0;
    
    /*if (Input::pressed(Input::Key::O))
        cam->scale += 0.25f;
    if (Input::pressed(Input::Key::U) and cam->scale > 0.25f)
        cam->scale -= 0.25f;*/
    
    return true;
}
