//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_camera.h"

#include <glm/ext.hpp>

#include "ftime.h"
#include "log.h"
#include "r_renderer.h"
#include "controller_list.h"

using namespace Verse;

namespace {
    Vec2 pixel_perfect_move;
    Vec2f extra_move;

    Vec2f shake_vec;
    ui32 shake_timer = 0;
    float shake_strength;
}

void System::Camera::init(Component::Camera* camera) {
    camera->vel = Vec2f(0,0);
}

void System::Camera::update(Config &c) {
    for (EntityID e : SceneView<Component::Camera>(*c.active_scene)) {
        Component::Camera* cam = c.active_scene->getComponent<Component::Camera>(e);
        if (cam != c.active_camera)
            continue;
        
        cam->controller();
        
        Vec2f total = cam->remainder + cam->vel * c.physics_delta;
        Vec2 to_move = Vec2(floor(total.x), floor(total.y));
        cam->pos += to_move;
        cam->remainder = total - to_move.to_float();
        
        pixel_perfect_move = Vec2(0.5f * c.resolution.x - cam->pos.x, 0.5f * c.resolution.y - cam->pos.y);
        extra_move = (c.use_subpixel_cam) ? Vec2f(-cam->remainder.x, cam->remainder.y) * c.render_scale : Vec2f(0,0);
        
        if(checkTimer(shake_timer)) {
            shake_vec = Vec2f(0,0);
            shake_timer = 0;
            shake_strength = 0;
        }
        
        if(shake_timer != 0)
            shake_vec = (Vec2f(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX) * 2.0f - Vec2f::one) *
                         c.render_scale * shake_strength - extra_move;
        
        cam->m_pixel = glm::translate(glm::mat4(1.0f), glm::vec3(pixel_perfect_move.x, pixel_perfect_move.y, 0.0f));
        cam->m_extra = glm::translate(glm::mat4(1.0f), glm::vec3(extra_move.x + shake_vec.x, extra_move.y + shake_vec.y, 0.0f));
    }
}

void System::Camera::shake(Config &c, ui16 ms, float strength) {
    shake_timer = setTimer(ms);
    shake_strength = strength;
    if (strength > c.render_scale * BORDER_WIDTH) {
        log::warn("You tried to use camera shake with a higher shake than the border width. Please adjust one of them.");
        shake_strength = c.render_scale * BORDER_WIDTH;
    }
}

void System::Camera::load(EntityID eid, YAML::Node &entity, Scene *s, Config &c) {
    Component::Camera* camera = s->addComponent<Component::Camera>(eid);
    if (entity["camera"]["pos"])
        camera->pos = entity["camera"]["pos"].as<Vec2>();
    camera->bounds = Rect2(0,0,0,0);
    if (entity["camera"]["bounds"]) {
        if (entity["camera"]["bounds"].IsMap()) {
            camera->bounds = entity["camera"]["bounds"].as<Rect2>();
        } else {
            if (entity["camera"]["bounds"].as<str>() == "scene")
                camera->bounds = Rect2(s->size * 0.5f, s->size);
            if (entity["camera"]["bounds"].as<str>() == "none")
                camera->bounds = Rect2(0,0,0,0);
        }
    }
    if (entity["camera"]["controller"]) {
        if (entity["camera"]["controller"].as<str>() == "actor")
            camera->controller = CAMERA_ACTOR_CONTROLLER;
        if (entity["camera"]["controller"].as<str>() == "free")
            camera->controller = CAMERA_FREE_CONTROLLER;
    }
    System::Camera::init(camera);
}
