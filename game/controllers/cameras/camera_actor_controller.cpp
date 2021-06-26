//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "camera_actor_controller.h"

#include "game.h"
#include "input.h"
#include "log.h"
#include "ftime.h"
#include "s_collider.h"
#include "controller_list.h"

#define CAM_EPSILON 0.5f
#define LA_SPEED 2.5f

using namespace Verse;

namespace {
    int la_dir = 0;
    float la_percent = 0.0f;
    Vec2 la_bounds = Vec2(60, 40);

    int input = 0;
}

bool Controller::Camera::Actor::controller(Config &c, EntityID eid) {
    Component::Camera* cam = c.active_scene->getComponent<Component::Camera>(eid);
    
    Component::Collider* collider = c.active_scene->getComponent<Component::Collider>(eid);
    
    input = 0;
    if (Input::down(Input::Key::Left) or Input::down(Input::Key::A))
        input = -1;
    if (Input::down(Input::Key::Right) or Input::down(Input::Key::D))
        input = 1;
    
    cam->target_pos = collider->transform.pos.to_float() + collider->transform.size.to_float() * 0.5f;
    
    lookahead(c, cam, input);
    
    checkBounds(c, cam);
    
    return true;
}

void Controller::Camera::Actor::lookahead(Config &c, Component::Camera *cam, int input) {
    la_percent = (input != 0 and input != la_dir) ? 1.0f - la_percent : la_percent;
    la_dir = (input != 0) ? input : la_dir;
    
    if (input != 0)
        la_percent += (la_percent < 1.0f) ? LA_SPEED * c.physics_delta * (1.0f - pow(la_percent, 2)) : 0.0f;
    
    cam->target_pos.x += la_bounds.x * la_dir * (la_percent - 0.5f);
}

void Controller::Camera::Actor::checkBounds(Config &c, Component::Camera* cam) {
    if (cam->bounds.w != 0) {
        if (cam->target_pos.x + (0.5f * c.resolution.x) > *cam->bounds.x + 0.5f * *cam->bounds.w)
            cam->target_pos.x = *cam->bounds.x + 0.5f * *cam->bounds.w - 0.5f * c.resolution.x;
        if (cam->target_pos.x - (0.5f * c.resolution.x) < *cam->bounds.x - 0.5f * *cam->bounds.w)
            cam->target_pos.x = *cam->bounds.x - 0.5f * *cam->bounds.w + 0.5f * c.resolution.x;
    }
    
    if (cam->bounds.h != 0) {
        if (cam->target_pos.y + (0.5f * c.resolution.y) > *cam->bounds.y + 0.5f * *cam->bounds.h)
            cam->target_pos.y = *cam->bounds.y + 0.5f * *cam->bounds.h - 0.5f * c.resolution.y;
        if (cam->target_pos.y - (0.5f * c.resolution.y) < *cam->bounds.y - 0.5f * *cam->bounds.h)
            cam->target_pos.y = *cam->bounds.y - 0.5f * *cam->bounds.h + 0.5f * c.resolution.y;
    }
}
