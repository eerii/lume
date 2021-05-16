//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "camera_free_controller.h"

#include <glm/ext.hpp>

#include "input.h"

using namespace Verse;

bool Controller::Camera::Free::controller(Config &c, EntityID eid) {
    Component::Camera* cam = c.active_scene->getComponent<Component::Camera>(eid);
    
    if (Input::down(Input::Key::J))
        cam->vel.x = -6;
    if (Input::down(Input::Key::L))
        cam->vel.x = 6;
    if (Input::down(Input::Key::I))
        cam->vel.y = -6;
    if (Input::down(Input::Key::K))
        cam->vel.y = 6;
    
    if (!Input::down(Input::Key::J) && !Input::down(Input::Key::L))
        cam->vel.x = 0;
    if (!Input::down(Input::Key::I) && !Input::down(Input::Key::K))
        cam->vel.y = 0;
    
    if (Input::pressed(Input::Key::O))
        cam->scale += 0.25f;
    if (Input::pressed(Input::Key::U) and cam->scale > 0.25f)
        cam->scale -= 0.25f;
    
    cam->pos = cam->target_pos.to_float();
    
    Vec2 pixel_perfect_move = Vec2(floor(0.5f * c.resolution.x - cam->pos.x), floor(0.5f * c.resolution.y - cam->pos.y));
    Vec2f extra_move = (((c.resolution * 0.5f).to_float() - cam->pos) - pixel_perfect_move.to_float()) * c.render_scale;
    
    cam->m_pixel = glm::translate(glm::mat4(1.0f), glm::vec3(pixel_perfect_move.x, pixel_perfect_move.y, 0.0f));
    cam->m_extra = glm::translate(glm::mat4(1.0f), glm::vec3(extra_move.x, extra_move.y, 0.0f));
    
    cam->m_pixel = glm::scale(cam->m_pixel, glm::vec3(cam->scale, cam->scale, 1.0f));
    
    return true;
}
