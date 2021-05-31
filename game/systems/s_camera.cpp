//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_camera.h"
#include <glm/ext.hpp>

using namespace Verse;

void System::Camera::init(Component::Camera* camera) {
    camera->vel = Vec2f(0,0);
}

void System::Camera::update(Config &c) {
    for (EntityID e : SceneView<Component::Camera>(*c.active_scene)) {
        Component::Camera* cam = c.active_scene->getComponent<Component::Camera>(e);
        if (cam != c.active_camera)
            break;
        
        cam->controller();
        
        cam->pos += cam->vel * c.physics_delta;
        
        Vec2 pixel_perfect_move = Vec2(floor(0.5f * c.resolution.x - cam->pos.x), floor(0.5f * c.resolution.y - cam->pos.y));
        Vec2f extra_move = (((c.resolution.to_float() * 0.5f) - cam->pos) - pixel_perfect_move.to_float()) * c.render_scale;
        
        cam->m_pixel = glm::translate(glm::mat4(1.0f), glm::vec3(pixel_perfect_move.x, pixel_perfect_move.y, 0.0f));
        cam->m_extra = glm::translate(glm::mat4(1.0f), glm::vec3(extra_move.x, extra_move.y, 0.0f));
    }
}
