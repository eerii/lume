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
    glm::mat4 mat_extra_camera;
    Component::Camera *cam; //Active Camera
    Rect2 bounds;

    Vec2 focus_pos;
    float look_ahead = 32;
    float la_speed = 1;
    float curr_la, target_la;
    bool is_la_stopped;
}

void System::Camera::init(Component::Camera* camera, Vec2f pos, Vec2f size) {
    camera->centre = pos;
    camera->vel = Vec2f(0,0);
    camera->l = pos.x - (size.x / 2.0);
    camera->r = pos.x + (size.x / 2.0);
    camera->t = pos.y - (size.y / 2.0);
    camera->b = pos.y + (size.y / 2.0);
}

void System::Camera::setActive(Component::Camera *camera) {
    Graphics::Renderer::bindCamera(&mat_active_camera, &mat_extra_camera, &camera->centre);
    cam = camera;
    bounds = Rect2(0,0,0,0);
}

void System::Camera::setActive(Component::Camera *camera, Rect2 cam_bounds) {
    setActive(camera);
    bounds = cam_bounds;
}

void System::Camera::setActive(Component::Camera *camera, Scene &scene) {
    setActive(camera);
    bounds = Rect2(scene.size * 0.5f, scene.size);
}

void System::Camera::update(Config &c, Scene &scene) {
    for (EntityID e : SceneView<Component::Camera>(scene)) {
        Component::Camera* camera = scene.getComponent<Component::Camera>(e);
        if (camera != cam)
            break;
        
        Component::Collider* collider = scene.getComponent<Component::Collider>(e);
        Component::Actor* actor = scene.getComponent<Component::Actor>(e);
        
        System::Camera::move(c, collider->transform.pos().to_float() + collider->transform.size().to_float() / 2.0f, //TODO: CHANGE, use camera's pos
                             (actor != nullptr) ? ((actor->vel.x != 0) ? sign(actor->vel.x) : 0) : 0);
    }
}

void System::Camera::move(Config &c, Vec2f pos, int input) {
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
    
    cam->centre = focus_pos.to_float();
    
    Vec2 pixel_perfect_move = Vec2(floor(0.5f * c.resolution.x - cam->centre.x), floor(0.5f * c.resolution.y - cam->centre.y));
    Vec2f extra_move = (((c.resolution * 0.5f).to_float() - cam->centre) - pixel_perfect_move.to_float()) * c.render_scale;
    
    mat_active_camera = glm::translate(glm::mat4(1.0f), glm::vec3(pixel_perfect_move.x, pixel_perfect_move.y, 0.0f));
    mat_extra_camera = glm::translate(glm::mat4(1.0f), glm::vec3(extra_move.x, extra_move.y, 0.0f));
}

void System::Camera::updatePoints(Config &c, Vec2f &pos) {
    cam->vel = Vec2f(0,0);
    
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
    if (bounds.w != 0) {
        if (focus_pos.x + (0.5f * c.resolution.x) > bounds.x + 0.5f * bounds.w) {
            focus_pos.x = bounds.x + 0.5f * bounds.w - 0.5f * c.resolution.x;
        }
        if (focus_pos.x - (0.5f * c.resolution.x) < bounds.x - 0.5f * bounds.w) {
            focus_pos.x = bounds.x - 0.5f * bounds.w + 0.5f * c.resolution.x;
        }
    }
    
    if (bounds.h != 0) {
        if (focus_pos.y + (0.5f * c.resolution.y) > bounds.y + 0.5f * bounds.h) {
            focus_pos.y = bounds.y + 0.5f * bounds.h - 0.5f * c.resolution.y;
        }
        if (focus_pos.y - (0.5f * c.resolution.y) < bounds.y - 0.5f * bounds.h) {
            focus_pos.y = bounds.y - 0.5f * bounds.h + 0.5f * c.resolution.y;
        }
    }
}
