//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_light.h"
#include "r_opengl.h"
#include "time.h"

#define LIGHT_PERIOD 5
#define LIGHT_VARIATION 5
#define LIGHT_CENTER_RADIUS 0.2

using namespace Verse;

namespace {
    std::vector<EntityID> light_entities;
    std::vector<glm::vec4> light_sources;
}

void System::Light::render(Scene &scene, Config &c) {
    for (EntityID e : SceneView<Component::Light>(scene)) {
        Component::Light* light = scene.getComponent<Component::Light>(e);
        Component::Texture* tex = scene.getComponent<Component::Texture>(e);
        
        std::vector<EntityID>::iterator it = std::find(light_entities.begin(), light_entities.end(), e);
        if (it != light_entities.end()) {
            long i = it - light_entities.begin();
            
            light_sources[i][0] = light->pos.x;
            light_sources[i][1] = light->pos.y;
            if (tex != nullptr) { //This is to render the light relative to the texture
                light_sources[i][0] += tex->transform.pos.x;
                light_sources[i][1] += tex->transform.pos.y;
            }
            
            light_sources[i][0] /= c.resolution.x;
            light_sources[i][1] /= c.resolution.y;
            
            light_sources[i][2] = (light->radius + sin(Time::current * 0.001 * LIGHT_PERIOD) * LIGHT_VARIATION) / c.resolution.y;
            light_sources[i][3] = light_sources[i][2] * LIGHT_CENTER_RADIUS;
        } else {
            light_entities.push_back(e);
            light_sources.push_back({light->pos.x, light->pos.y, light->radius, light->radius * LIGHT_CENTER_RADIUS});
        }
    }
}

std::vector<glm::vec4> System::Light::getLight() {
    return light_sources;
}
