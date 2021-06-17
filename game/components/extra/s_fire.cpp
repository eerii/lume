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
#ifndef __EMSCRIPTEN__
    fire->p_data = (ui8*)malloc((fire->transform.w+1) * (fire->transform.h+1));
#else
    fire->p_data = (ui8*)malloc((fire->transform.w+1) * (fire->transform.h+1) * 4);
#endif
    
    Graphics::Texture::createGradient(fire->transform.w, fire->g_tex);
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
                                                 fire->levels,
                                                 fire->p_data,
                                                 fire->p_tex);
            
            fire->noise_time = 0;
        }
        if (fire->noise_offset > 256)
            fire->noise_offset = 0;
        
        Graphics::Renderer::renderFire(c, fire->transform, fire->p_tex, fire->g_tex, fire->flame_tex, fire->layer);
    }
}

void System::Fire::load(EntityID eid, YAML::Node &entity, Scene *s, Config &c) {
    Component::Fire* fire = s->addComponent<Component::Fire>(eid);
    if (entity["fire"]["transform"])
        fire->transform = entity["fire"]["transform"].as<Rect2>();
    if (entity["fire"]["offset"])
        fire->offset = entity["fire"]["offset"].as<Vec2>();
    if (entity["fire"]["dir"])
        fire->dir = entity["fire"]["dir"].as<Vec2>();
    if (entity["fire"]["fps"])
        fire->fps = (ui8)entity["fire"]["fps"].as<int>();
    if (entity["fire"]["freq"])
        fire->freq = entity["fire"]["freq"].as<float>();
    if (entity["fire"]["levels"])
        fire->levels = entity["fire"]["levels"].as<int>();
    if (not entity["fire"]["res"]) {
        log::error("You created a fire component for " + s->getName(eid) + " but it has no res for the texture");
        s->removeEntity(eid);
        return;
    }
    fire->layer = (entity["fire"]["layer"]) ? entity["fire"]["layer"].as<int>() : 0;
    fire->flame_tex_res = entity["fire"]["res"].as<str>();
    Graphics::Texture::loadTexture(fire->flame_tex_res, fire->flame_tex);
    System::Fire::init(fire);
}
