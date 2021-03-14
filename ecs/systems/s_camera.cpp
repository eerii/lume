//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "s_camera.h"
#include "r_renderer.h"

using namespace Verse;

namespace {
    glm::mat4 mat_active_camera;
    Component::Camera *cam; //Active Camera

    float look_ahead;
    Vec2 smooth_time;
}

void System::Camera::init(Component::Camera* camera, Vec2 pos, Vec2 size) {
    camera->centre = pos;
    camera->vel = Vec2(0,0);
    camera->l = pos.x - (size.x / 2.0);
    camera->r = pos.x + (size.x / 2.0);
    camera->t = pos.y - (size.y / 2.0);
    camera->b = pos.y + (size.y / 2.0);
}

void System::Camera::setActive(Component::Camera *camera) {
    Graphics::Renderer::GL::useCamera(&mat_active_camera, &camera->centre);
    cam = camera;
}

void System::Camera::move(Config &c, Vec2 pos) {
    cam->vel = Vec2(0,0);
    
    if (pos.x < cam->l)
        cam->vel.x = pos.x - cam->l;
    else if (pos.x > cam->r)
        cam->vel.x = pos.x - cam->r;
    cam->l += cam->vel.x;
    cam->r += cam->vel.x;
    
    if (pos.y < cam->t)
        cam->vel.y = pos.y - cam->t;
    else if (pos.y > cam->b)
        cam->vel.y = pos.y - cam->b;
    cam->t += cam->vel.y;
    cam->b += cam->vel.y;
    
    cam->centre = Vec2(floor(c.resolution.x - (cam->l+cam->r)/2.0f), floor(c.resolution.y - (cam->t+cam->b)/2.0f));
    
    mat_active_camera = glm::translate(glm::mat4(1.0f), glm::vec3((cam->centre.x - 0.5 * c.resolution.x) / c.render_scale,
                                                                  (cam->centre.y - 0.5 * c.resolution.y) / c.render_scale, 0.0f));
}
