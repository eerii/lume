//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_fire.h"
#include "time.h"
#include "r_pipeline.h"
#include "r_textures.h"
#include "r_renderer.h"

using namespace Verse;

namespace {
    std::map<EntityID, float> noise_time;
    std::map<EntityID, ui16> noise_offset;

    float refresh_rate = 0;
}

void System::Fire::init(Component::Fire *fire) {
    fire->p_data = new ui8[fire->transform.w * fire->transform.h];
}

void System::Fire::render(Scene &scene, Config &c) {
    if (refresh_rate == 0)
        refresh_rate = Graphics::getRefreshRate();
    
    for (EntityID e : SceneView<Component::Fire>(scene)) {
        Component::Fire* fire = scene.getComponent<Component::Fire>(e);
        
        if (noise_time.count(e) == 0)
            noise_time[e] = 0;
        if (noise_offset.count(e) == 0)
            noise_offset[e] = 0;
        
        noise_time[e] += ((float)Time::delta / 1000.0f) * c.game_speed;
        if (noise_time[e] > (1.0f / (float)fire->fps)) {
            noise_offset[e]++;
            
            /*Graphics::Texture::offsetWhiteNoise(fire->transform.w,
                                                fire->w_data,
                                                fire->w_tex);*/
            Graphics::Texture::createPerlinNoise(fire->transform.w,
                                                 fire->dir * noise_offset[e],
                                                 fire->freq,
                                                 fire->octaves,
                                                 fire->seed,
                                                 fire->p_data,
                                                 fire->p_tex);
            
            noise_time[e] = 0;
        }
        if (noise_offset[e] > 256)
            noise_offset[e] = 0;
        
        Graphics::Renderer::renderFire(fire->transform, fire->p_tex, fire->flame_tex, c, fire->layer);
    }
}
