//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "camera_actor_controller.h"
#include "input.h"

#define EPSILON 0.1

using namespace Verse;

namespace {
    float curr_la, target_la;
    bool is_la_stopped;
}

bool Controller::Camera::Actor::controller(Scene &s, Config &c, EntityID eid) {
    Component::Camera* cam = s.getComponent<Component::Camera>(eid);
    
    Component::Actor* actor = s.getComponent<Component::Actor>(eid);
    int input = (actor->vel.x != 0) ? sign(actor->vel.x) : 0;
    
    is_la_stopped = (input != 0 and input == sign(cam->vel.x)) ? false : is_la_stopped;
    
    if (not is_la_stopped) {
        if (input == 0) {
            is_la_stopped = true;
            target_la = curr_la + (sign(cam->vel.x) * cam->look_ahead - curr_la) * 0.25;
        } else if (cam->vel.x != 0) {
            target_la = sign(cam->vel.x) * cam->look_ahead;
        }
    }
    
    if (abs(target_la - curr_la) > EPSILON) {
        curr_la += cam->la_speed * sign(cam->vel.x) * ((target_la - curr_la) / target_la);
    }
    
    cam->target_pos.x += curr_la;
    
    checkBounds(c, cam);
    
    cam->pos = cam->target_pos.to_float();
    
    Vec2 pixel_perfect_move = Vec2(floor(0.5f * c.resolution.x - cam->pos.x), floor(0.5f * c.resolution.y - cam->pos.y));
    Vec2f extra_move = (((c.resolution * 0.5f).to_float() - cam->pos) - pixel_perfect_move.to_float()) * c.render_scale;
    
    cam->m_pixel = glm::translate(glm::mat4(1.0f), glm::vec3(pixel_perfect_move.x, pixel_perfect_move.y, 0.0f));
    cam->m_extra = glm::translate(glm::mat4(1.0f), glm::vec3(extra_move.x, extra_move.y, 0.0f));
    
    return true;
}

void Controller::Camera::Actor::checkBounds(Config &c, Component::Camera* cam) {
    if (cam->bounds.w != 0) {
        if (cam->target_pos.x + (0.5f * c.resolution.x) > cam->bounds.x + 0.5f * cam->bounds.w) {
            cam->target_pos.x = cam->bounds.x + 0.5f * cam->bounds.w - 0.5f * c.resolution.x;
        }
        if (cam->target_pos.x - (0.5f * c.resolution.x) < cam->bounds.x - 0.5f * cam->bounds.w) {
            cam->target_pos.x = cam->bounds.x - 0.5f * cam->bounds.w + 0.5f * c.resolution.x;
        }
    }
    
    if (cam->bounds.h != 0) {
        if (cam->target_pos.y + (0.5f * c.resolution.y) > cam->bounds.y + 0.5f * cam->bounds.h) {
            cam->target_pos.y = cam->bounds.y + 0.5f * cam->bounds.h - 0.5f * c.resolution.y;
        }
        if (cam->target_pos.y - (0.5f * c.resolution.y) < cam->bounds.y - 0.5f * cam->bounds.h) {
            cam->target_pos.y = cam->bounds.y - 0.5f * cam->bounds.h + 0.5f * c.resolution.y;
        }
    }
}
