//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_light.h"

#include <map>

#include "ftime.h"
#include "r_opengl.h"

#define LIGHT_PERIOD 5
#define LIGHT_VARIATION 5
#define LIGHT_CENTER_RADIUS 0.25

using namespace Verse;



#include "log.h"

namespace {
    std::map<EntityID, glm::vec4> light_sources;
}

void System::Light::render(Config &c, ui8 pid) {
    for (EntityID e : SceneView<Component::Light>(*c.active_scene)) {
        Component::Light* light = c.active_scene->getComponent<Component::Light>(e);
        Component::Texture* tex = c.active_scene->getComponent<Component::Texture>(e);
        
        float variation = sin(Time::current * 0.001f * LIGHT_PERIOD * c.game_speed) * LIGHT_VARIATION;
        float radius_with_var = ((light->radius + variation) > 2.0f) ? light->radius + variation : 2.0f; //Prevent visual errors
        
        light_sources[e] = glm::vec4(light->pos.x, light->pos.y, radius_with_var, light->radius * LIGHT_CENTER_RADIUS);
        if (tex != nullptr)
            light_sources[e] += glm::vec4(tex->transform.x, tex->transform.y, 0, 0);
        
        light_sources[e].x /= c.resolution.x;
        light_sources[e].y = 1.0f - light_sources[e].y / c.resolution.y;
        light_sources[e].z /= c.resolution.x;
        light_sources[e].w /= c.resolution.x;
        
        light_sources[e].x += (0.5 - (c.active_camera->pos.x / c.resolution.x));
        light_sources[e].y -= (0.5 - (c.active_camera->pos.y / c.resolution.y));
    }
    
    std::vector<glm::vec4> light_data;
    for (const auto& [_, l] : light_sources) {
        light_data.push_back(l);
    }
    
    glUniform4fv(glGetUniformLocation(pid, "light"), (int)(light_sources.size()), reinterpret_cast<GLfloat *>(light_data.data()));
    glUniform1i(glGetUniformLocation(pid, "light_size"), (int)(light_sources.size()));
    glUniform1f(glGetUniformLocation(pid, "light_distortion"), (float)c.resolution.x / (float)c.resolution.y);
}

void System::Light::clean() {
    light_sources = {};
}
