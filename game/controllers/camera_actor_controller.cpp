//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "camera_actor_controller.h"

#include "input.h"
#include "log.h"
#include "ftime.h"
#include "s_collider.h"

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
    Component::Actor* actor = c.active_scene->getComponent<Component::Actor>(eid);
    
    
    Component::Collider* platform_collider = nullptr;
    Component::Actor* platform_actor = nullptr;
    collider->transform += Vec2::j;
    System::Collider::CollisionInfo collision_info = System::Collider::checkCollisions(c, eid);
    for (System::Collider::CollisionInfoPair collision : collision_info) {
        if (collision.second[System::Collider::Layers::Platform]) {
            platform_collider = c.active_scene->getComponent<Component::Collider>(collision.first);
            
            bool above = collider->transform.y + collider->transform.h <= platform_collider->transform.y + 1;
            if (above)
                platform_actor = c.active_scene->getComponent<Component::Actor>(collision.first);
        }
    }
    collider->transform -= Vec2::j;
    
    
    cam->target_pos = collider->transform.pos() + collider->transform.size() * 0.5f;
    if (platform_actor != nullptr and actor->patrol_points.size() > 0) {
        int next_patrol_point = (actor->current_patrol_point + 1) % actor->patrol_points.size();
        cam->target_pos = platform_actor->patrol_points[next_patrol_point];
    }
    
    int prev_input = input;
    input = 0;
    if (Input::down(Input::Key::Left) or Input::down(Input::Key::A))
        input = -1;
    if (Input::down(Input::Key::Right) or Input::down(Input::Key::D))
        input = 1;
    if (input != prev_input)
        cam->vel.x = 0;
    
    if (platform_actor == nullptr)
        lookahead(c, cam, input);
    
    checkBounds(c, cam);
    
    float distance = cam->target_pos.x - cam->pos.x;
    cam->vel.x = (platform_actor == nullptr) ? sign(distance) * pow(abs(distance), 1.5f) : sign(distance) * platform_actor->max_move_speed;
    
    if (abs(distance) < CAM_EPSILON)
        cam->vel.x = 0;
    
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
        if (cam->target_pos.x + (0.5f * c.resolution.x) > cam->bounds.x + 0.5f * cam->bounds.w)
            cam->target_pos.x = cam->bounds.x + 0.5f * cam->bounds.w - 0.5f * c.resolution.x;
        if (cam->target_pos.x - (0.5f * c.resolution.x) < cam->bounds.x - 0.5f * cam->bounds.w)
            cam->target_pos.x = cam->bounds.x - 0.5f * cam->bounds.w + 0.5f * c.resolution.x;
    }
    
    if (cam->bounds.h != 0) {
        if (cam->target_pos.y + (0.5f * c.resolution.y) > cam->bounds.y + 0.5f * cam->bounds.h)
            cam->target_pos.y = cam->bounds.y + 0.5f * cam->bounds.h - 0.5f * c.resolution.y;
        if (cam->target_pos.y - (0.5f * c.resolution.y) < cam->bounds.y - 0.5f * cam->bounds.h)
            cam->target_pos.y = cam->bounds.y - 0.5f * cam->bounds.h + 0.5f * c.resolution.y;
    }
}
