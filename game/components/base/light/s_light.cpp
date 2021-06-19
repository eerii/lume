//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_light.h"

#include <map>

#include "ftime.h"
#include "r_opengl.h"
#include "r_renderer.h"

#include "gui.h"
#include "gui_types.h"

#define LIGHT_PERIOD 5
#define LIGHT_VARIATION 5
#define LIGHT_CENTER_RADIUS 0.25

using namespace Verse;

void System::Light::render(Config &c, ui8 pid) {
    std::map<EntityID, glm::vec4> light_sources;
    
    for (EntityID e : SceneView<Component::Light>(*c.active_scene)) {
        Component::Light* light = c.active_scene->getComponent<Component::Light>(e);
        Component::Texture* tex = c.active_scene->getComponent<Component::Texture>(e);
        
        float variation = sin(time() * 0.001f * LIGHT_PERIOD * c.game_speed) * LIGHT_VARIATION;
        float radius_with_var = ((light->radius + variation) > 2.0f) ? light->radius + variation : 2.0f; //Prevent visual errors
        
        light_sources[e] = glm::vec4(light->pos.x, light->pos.y, radius_with_var, light->radius * LIGHT_CENTER_RADIUS);
        if (tex != nullptr)
            light_sources[e] += glm::vec4(*tex->transform.x, *tex->transform.y, 0, 0);
        
        light_sources[e].x /= c.resolution.x + 2.0f*BORDER_WIDTH;
        light_sources[e].y = 1.0f - light_sources[e].y / (c.resolution.y + 2.0f*BORDER_WIDTH);
        light_sources[e].z /= c.resolution.x + 2.0f*BORDER_WIDTH;
        light_sources[e].w /= c.resolution.x + 2.0f*BORDER_WIDTH;
        
        light_sources[e].x += (0.5f * c.resolution.x - BORDER_WIDTH - (float)floor(c.active_camera->pos.x)) / (c.resolution.x + 2.0f*BORDER_WIDTH);
        light_sources[e].y -= (0.5f * c.resolution.y - BORDER_WIDTH - (float)floor(c.active_camera->pos.y)) / (c.resolution.y + 2.0f*BORDER_WIDTH);
        
    }
    
    std::vector<glm::vec4> light_data;
    for (const auto& [_, l] : light_sources) {
        light_data.push_back(l);
    }
    
    glUniform4fv(glGetUniformLocation(pid, "light"), (int)(light_sources.size()), reinterpret_cast<GLfloat *>(light_data.data()));
    glUniform1i(glGetUniformLocation(pid, "light_size"), (int)(light_sources.size()));
    glUniform1f(glGetUniformLocation(pid, "light_distortion"), (float)(c.resolution.x + 2*BORDER_WIDTH) / (float)(c.resolution.y + 2*BORDER_WIDTH));
}

void System::Light::load(EntityID eid, YAML::Node &entity, Scene *s, Config &c) {
    Component::Light* light = s->addComponent<Component::Light>(eid);
    if (entity["light"]["pos"])
        light->pos = entity["light"]["pos"].as<Vec2>();
    if (entity["light"]["radius"])
        light->radius = entity["light"]["radius"].as<int>();
}

void System::Light::gui(Config &c, EntityID eid) {
#ifndef DISABLE_GUI
    
#endif
}
