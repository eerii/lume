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
#define DAMPNESS 0.05 //Lower is slower

using namespace Verse;

namespace {
    glm::mat4 mat_active_camera;
    Component::Camera *cam; //Active Camera

    Vec2 focus_pos;
    float look_ahead = 32;
    float curr_la, target_la;
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
    
    if (cam->vel.x != 0) {
        target_la = -sign(cam->vel.x) * look_ahead;
    }
    
    //Smooth transition
    if (abs(target_la - curr_la) > EPSILON) {
        curr_la += (target_la - curr_la) * DAMPNESS;
    }
    
    cam->centre = focus_pos + Vec2(curr_la, 0);
    
    mat_active_camera = glm::translate(glm::mat4(1.0f), glm::vec3((cam->centre.x - 0.5 * c.resolution.x) / c.render_scale,
                                                                  (cam->centre.y - 0.5 * c.resolution.y) / c.render_scale, 0.0f));
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
    
    focus_pos = Vec2(floor(c.resolution.x - (cam->l+cam->r)/2.0f), floor(c.resolution.y - (cam->t+cam->b)/2.0f));
}
