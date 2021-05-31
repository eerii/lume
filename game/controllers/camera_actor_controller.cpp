//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "camera_actor_controller.h"

#include "input.h"

#include "log.h"

#define EPSILON 3.0f
#define DIST_POW 1.8f

#define CAM_SPEED 1.0f
#define LA_SPEED 2.5f

using namespace Verse;

namespace {
    int la_dir = 0;
    float la_percent = 0.0f;
    Vec2 la_bounds = Vec2(40, 40);
}

bool Controller::Camera::Actor::controller(Config &c, EntityID eid) {
    Component::Camera* cam = c.active_scene->getComponent<Component::Camera>(eid);
    
    Component::Collider* collider = c.active_scene->getComponent<Component::Collider>(eid);
    Component::Actor* actor = c.active_scene->getComponent<Component::Actor>(eid);
    
    cam->target_pos = collider->transform.pos().to_float();
    
    int input = (actor->vel.x != 0) ? sign(actor->vel.x) : 0;
    lookahead(c, cam, input);
    
    checkBounds(c, cam);
    
    Vec2f diff = (cam->target_pos - cam->pos);
    Vec2f dir = diff.normal();
    float dist = (diff.length() > EPSILON - actor->remainder.x) ? pow(diff.length(), DIST_POW) : 0.0f;
    cam->vel = dir * dist * CAM_SPEED;
    
    return true;
}

void Controller::Camera::Actor::lookahead(Config &c, Component::Camera *cam, int input) {
    la_percent = (input != 0 and input != la_dir) ? 0.0f : la_percent;
    la_dir = (input != 0) ? input : la_dir;
    
    la_percent += (la_percent < 1.0f) ? LA_SPEED * c.physics_delta * (1.0f - pow(la_percent, 3)) : 0.0f;
    
    cam->target_pos.x += la_bounds.x * la_dir * (la_percent - 0.5f);
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
