//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_camera.h"

using namespace Verse;

void System::Camera::init(Component::Camera* camera) {
    camera->vel = Vec2f(0,0);
    camera->l = camera->pos.x - ((float)camera->focus_size.x / 2.0f);
    camera->r = camera->pos.x + ((float)camera->focus_size.x / 2.0f);
    camera->t = camera->pos.y - ((float)camera->focus_size.y / 2.0f);
    camera->b = camera->pos.y + ((float)camera->focus_size.y / 2.0f);
}

void System::Camera::update(Config &c) {
    for (EntityID e : SceneView<Component::Camera>(*c.active_scene)) {
        Component::Camera* camera = c.active_scene->getComponent<Component::Camera>(e);
        if (camera != c.active_camera)
            break;
        
        updatePoints(c, camera->target_pos.to_float());
        
        camera->controller();
    }
}

void System::Camera::updatePoints(Config &c, Vec2f pos) {
    if (pos.x < c.active_camera->l)
        c.active_camera->vel.x = pos.x - c.active_camera->l;
    else if (pos.x > c.active_camera->r)
        c.active_camera->vel.x = pos.x - c.active_camera->r;
    c.active_camera->l += c.active_camera->vel.x;
    c.active_camera->r += c.active_camera->vel.x;
    
    if (pos.y < c.active_camera->t)
        c.active_camera->vel.y = pos.y - c.active_camera->t;
    else if (pos.y > c.active_camera->b)
        c.active_camera->vel.y = pos.y - c.active_camera->b;
    c.active_camera->t += c.active_camera->vel.y;
    c.active_camera->b += c.active_camera->vel.y;
    
    c.active_camera->target_pos = Vec2(floor((c.active_camera->l+c.active_camera->r)/2.0f),
                                       floor((c.active_camera->t+c.active_camera->b)/2.0f));
    c.active_camera->vel = Vec2f(0,0);
}
