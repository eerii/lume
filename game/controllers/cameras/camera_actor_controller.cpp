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
#include "fmath.h"

using namespace Verse;

namespace {
    float la_speed = 1.5f;
    Vec2 la_bounds = Vec2(60, 40);

    int la_dir = 0;
    float la_percent = 0.0f;
    float target_percent = 1.0f;

    float la_pos = 0.0f;
    float la_vel = 0.0f;

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
    
    if (c.enable_lookahead)
        lookahead(c, cam, input);
    
    checkBounds(c, cam);
    
    return true;
}

void Controller::Camera::Actor::lookahead(Config &c, Component::Camera *cam, int input) {
    if (input != 0) {
        if (input != la_dir)
            la_percent = target_percent - la_percent;
        la_dir = input;
        la_percent += (la_percent < target_percent) ? la_speed * c.physics_delta * (target_percent - pow(la_percent, 1.5f)) : 0.0f;
    }
    
    float la_target = la_bounds.x * la_dir * (la_percent - 0.5f * target_percent) +
                      ((input == 0 and c.enable_smooth_panning) ? la_dir * 10.0f : 0.0f);
    
    if (c.enable_smooth_panning) {
        float prev_la_pos = la_pos;
        la_pos = Math::smoothDamp(la_pos, la_target, la_vel, 0.25, 100000, c.physics_delta);
        if (abs(prev_la_pos - la_pos) < 0.1f)
            la_pos = prev_la_pos;
    }
    
    cam->target_pos.x += (c.enable_smooth_panning) ? la_pos : la_target;
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
