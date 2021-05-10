//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_camera.h"
#include "r_renderer.h"
#include "time.h"
#include "game.h"

using namespace Verse;

namespace {
    Component::Camera *cam; //Active Camera
}

void System::Camera::init(Component::Camera* camera) {
    camera->vel = Vec2f(0,0);
    camera->l = camera->pos.x - ((float)camera->focus_size.x / 2.0f);
    camera->r = camera->pos.x + ((float)camera->focus_size.x / 2.0f);
    camera->t = camera->pos.y - ((float)camera->focus_size.y / 2.0f);
    camera->b = camera->pos.y + ((float)camera->focus_size.y / 2.0f);
}

void System::Camera::setActive(Component::Camera *camera) {
    cam = camera;
    Graphics::Renderer::bindCamera(&camera->m_pixel, &camera->m_extra, &camera->pos);
}

void System::Camera::update(Config &c, Scene &s) {
    for (EntityID e : SceneView<Component::Camera>(s)) {
        Component::Camera* camera = s.getComponent<Component::Camera>(e);
        if (camera != cam)
            break;
        
        updatePoints(c, cam->target_pos.to_float());
        
        camera->controller();
    }
}

void System::Camera::updatePoints(Config &c, Vec2f pos) {
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
    
    cam->target_pos = Vec2(floor((cam->l+cam->r)/2.0f), floor((cam->t+cam->b)/2.0f));
}
