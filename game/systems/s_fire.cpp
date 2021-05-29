//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_fire.h"

#include "ftime.h"

#include "r_pipeline.h"
#include "r_textures.h"
#include "r_renderer.h"

using namespace Verse;

namespace {
    float refresh_rate = 0;
}

void System::Fire::init(Component::Fire *fire) {
    fire->p_data = (ui8*)malloc((fire->transform.w+1) * (fire->transform.h+1));
}

void System::Fire::render(Config &c) {
    if (refresh_rate == 0)
        refresh_rate = Graphics::getRefreshRate();
    
    for (EntityID e : SceneView<Component::Fire>(*c.active_scene)) {
        Component::Fire* fire = c.active_scene->getComponent<Component::Fire>(e);
        
        fire->noise_time += ((float)Time::delta / 1000.0f) * c.game_speed;
        if (fire->noise_time > (1.0f / (float)fire->fps)) {
            fire->noise_offset++;
            Graphics::Texture::createPerlinNoise(fire->transform.size(),
                                                 fire->dir * fire->noise_offset,
                                                 fire->freq,
                                                 fire->octaves,
                                                 fire->seed,
                                                 fire->p_data,
                                                 fire->p_tex);
            
            fire->noise_time = 0;
        }
        if (fire->noise_offset > 256)
            fire->noise_offset = 0;
        
        Graphics::Renderer::renderFire(c, fire->transform, fire->p_tex, fire->flame_tex, fire->layer);
    }
}
