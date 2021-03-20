//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "s_camera.h"
#include "r_renderer.h"
#include "time.h"
#include "game.h"

#define EPSILON 0.1

using namespace Verse;

namespace {
    glm::mat4 mat_active_camera;
    Component::Camera *cam; //Active Camera
    Rect bounds;

    Vec2 focus_pos;
    float look_ahead = 32;
    float la_speed = 1;
    float curr_la, target_la;
    bool is_la_stopped;
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
    bounds = Rect(0,0,0,0);
}

void System::Camera::setActive(Component::Camera *camera, Rect cam_bounds) {
    setActive(camera);
    bounds = cam_bounds;
}

void System::Camera::setActive(Component::Camera *camera, Scene &scene) {
    setActive(camera);
    bounds = Rect(scene.size * 0.5f, scene.size);
}

void System::Camera::update(Config &c, Scene &scene) {
    for (EntityID e : SceneView<Component::Camera>(scene)) {
        Component::Camera* camera = scene.getComponent<Component::Camera>(e);
        if (camera != cam)
            break;
        
        Component::Collider* collider = scene.getComponent<Component::Collider>(e);
        Component::Actor* actor = scene.getComponent<Component::Actor>(e);
        
        System::Camera::move(c, collider->transform.pos + collider->transform.size / 2.0f,
                             (actor != nullptr) ? ((actor->vel.x != 0) ? sign(actor->vel.x) : 0) : 0);
    }
}

void System::Camera::move(Config &c, Vec2 pos, int input) {
    updatePoints(c, pos);
    
    is_la_stopped = (input != 0 and input == sign(cam->vel.x)) ? false : is_la_stopped;
    
    if (not is_la_stopped) {
        if (input == 0) {
            is_la_stopped = true;
            target_la = curr_la + (sign(cam->vel.x) * look_ahead - curr_la) * 0.25;
        } else if (cam->vel.x != 0) {
            target_la = sign(cam->vel.x) * look_ahead;
        }
    }
    
    if (abs(target_la - curr_la) > EPSILON) {
        curr_la += la_speed * sign(cam->vel.x) * ((target_la - curr_la) / target_la);
    }
    
    focus_pos.x += curr_la;
    
    checkBounds(c);
    
    cam->centre = focus_pos;
    
    mat_active_camera = glm::translate(glm::mat4(1.0f), glm::vec3((0.5f * c.resolution.x - cam->centre.x) / c.render_scale,
                                                                  (0.5f * c.resolution.y - cam->centre.y) / c.render_scale, 0.0f));
}

void System::Camera::updatePoints(Config &c, Vec2 &pos) {
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
    
    focus_pos = Vec2(floor((cam->l+cam->r)/2.0f), floor((cam->t+cam->b)/2.0f));
}

void System::Camera::checkBounds(Config &c) {
    if (bounds.size.x != 0) {
        if (focus_pos.x + (0.5f * c.resolution.x) > bounds.pos.x + 0.5f * bounds.size.x) {
            focus_pos.x = bounds.pos.x + 0.5f * bounds.size.x - 0.5f * c.resolution.x;
        }
        if (focus_pos.x - (0.5f * c.resolution.x) < bounds.pos.x - 0.5f * bounds.size.x) {
            focus_pos.x = bounds.pos.x - 0.5f * bounds.size.x + 0.5f * c.resolution.x;
        }
    }
    
    if (bounds.size.y != 0) {
        if (focus_pos.y + (0.5f * c.resolution.y) > bounds.pos.y + 0.5f * bounds.size.y) {
            focus_pos.y = bounds.pos.y + 0.5f * bounds.size.y - 0.5f * c.resolution.y;
        }
        if (focus_pos.y - (0.5f * c.resolution.y) < bounds.pos.y - 0.5f * bounds.size.y) {
            focus_pos.y = bounds.pos.y - 0.5f * bounds.size.y + 0.5f * c.resolution.y;
        }
    }
}
