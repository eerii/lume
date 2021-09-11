//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_noise.h"

#include <glm/ext.hpp>

#include "ftime.h"

#include "r_pipeline.h"
#include "r_textures.h"
#include "r_renderer.h"

#include "fmath.h"

#include "gui.h"
#include "gui_types.h"

using namespace Verse;

namespace {
    float v[16] = {
         0.0,  1.0,  0.0,  1.0,
         1.0,  1.0,  1.0,  1.0,
         0.0,  0.0,  0.0,  0.0,
         1.0,  0.0,  1.0,  0.0,
    };
    float vn[8] = {
        0.0,  1.0,
        1.0,  1.0,
        0.0,  0.0,
        1.0,  0.0,
    };
}

void System::Noise::init(Config &c, Scene *s, EntityID eid) {
    Component::Noise* noise = s->getComponent<Component::Noise>(eid);
    Component::Texture* tex = s->getComponent<Component::Texture>(eid);
    
    noise->size = tex->transform.size;
    noise->noise_data = std::vector<ui8>(noise->size.x * noise->size.y);
    
    Math::perlinNoise(noise->size, Vec2(0,0), noise->freq, noise->levels, noise->noise_data.data(), true);
    noise->noise_tex.w = noise->size.x; noise->noise_tex.h = noise->size.y;
    Graphics::Texture::createTexture(noise->noise_data.data(), noise->noise_tex, noise->size.x, noise->size.y, false);
}

void System::Noise::update(Config &c) {
    for (EntityID e : SceneView<Component::Noise>(*c.active_scene)) {
        Component::Noise* noise = c.active_scene->getComponent<Component::Noise>(e);
        Component::Texture* tex = c.active_scene->getComponent<Component::Texture>(e);
        
        if (not noise->enabled)
            continue;
        
        if (noise->size != tex->transform.size) {
            noise->size = tex->transform.size;
            noise->noise_data = std::vector<ui8>(noise->size.x * noise->size.y);
        }
        
        noise->noise_time += c.physics_delta;
        if (noise->fps != 0 and noise->noise_time > (1.0f / (float)noise->fps)) {
            noise->noise_offset++;
            noise->noise_tex.w = noise->size.x;
            noise->noise_tex.h = noise->size.y;
            Graphics::Texture::createPerlinNoise(noise->noise_data.data(), noise->noise_tex, noise->size,
                                                 noise->dir * noise->noise_offset, noise->freq, noise->levels);
            
            noise->noise_time = 0;
        }
        if (noise->noise_offset > 16384)
            noise->noise_offset = 0;
    }
}

void System::Noise::render(Config &c) {
    for (EntityID e : SceneView<Component::Noise>(*c.active_scene)) {
        Component::Noise* noise = c.active_scene->getComponent<Component::Noise>(e);
        Component::Texture* tex = c.active_scene->getComponent<Component::Texture>(e);
        Component::Animation* anim = c.active_scene->getComponent<Component::Animation>(e);
        
        if (not noise->enabled)
            continue;
        
        if (tex->res.size() == 0)
            continue;
        
        Vec2 tex_size = Vec2(1, (int)tex->layer.size());
        if (anim != nullptr)
            tex_size.x = anim->size;
        
        int i = tex_size.y - 1;
        
        std::vector<float> vertices(v, v + sizeof v / sizeof v[0]);
        
        bool has_anim = (anim != nullptr and
                         anim->frames.size() > 0 and
                         anim->frames.count(anim->curr_key) > 0 and
                         anim->frames[anim->curr_key].index.size() > anim->curr_frame);
        
        for (int j = 0; j < 4; j++) {
            vertices[j*4+2] /= (float)tex_size.x;
            vertices[j*4+3] = (vertices[j*4+3] + (float)i) / (float)tex_size.y;
            
            if (has_anim)
                vertices[j*4+2] += (float)anim->frames[anim->curr_key].index[anim->curr_frame] / (float)tex_size.x;
        }
        
        if (tex->is_reversed) {
            vertices[0*4+2] = vertices[1*4+2];
            vertices[1*4+2] = vertices[2*4+2];
            vertices[2*4+2] = vertices[0*4+2];
            vertices[3*4+2] = vertices[1*4+2];
        }
        
        Rect2 dst = Rect2 (tex->transform.pos + noise->offset + ((tex->offset.size() > i) ? tex->offset[i] : Vec2(0, 0)), noise->size);
        
        noise->mask_tex.gl_id = tex->data.gl_id;
        noise->mask_tex.model = Graphics::Renderer::matModel2D(dst.pos - Vec2(BORDER_WIDTH, BORDER_WIDTH), dst.size);
        noise->mask_tex.vertices = vertices;
        noise->mask_tex.layer = tex->layer[i];
        
        noise->noise_tex.vertices = std::vector<float>(vn, vn + sizeof vn / sizeof vn[0]);
        
        Graphics::Renderer::renderNoise(c, noise->mask_tex, noise->noise_tex);
    }
}

void System::Noise::load(EntityID eid, YAML::Node &entity, Scene *s, Config &c) {
    Component::Noise* noise = s->addComponent<Component::Noise>(eid);
    if (entity["noise"]["offset"])
        noise->offset = entity["noise"]["offset"].as<Vec2>();
    if (entity["noise"]["dir"])
        noise->dir = entity["noise"]["dir"].as<Vec2>();
    if (entity["noise"]["fps"])
        noise->fps = (ui8)entity["noise"]["fps"].as<int>();
    if (entity["noise"]["freq"])
        noise->freq = entity["noise"]["freq"].as<float>();
    if (entity["noise"]["levels"])
        noise->levels = entity["noise"]["levels"].as<int>();
    noise->enabled = (entity["noise"]["enabled"]) ? entity["noise"]["enabled"].as<bool>() : true;
    System::Noise::init(c, s, eid);
}

//TODO: Noise save

void System::Noise::gui(Config &c, EntityID eid) {
#ifndef DISABLE_GUI
    Component::Noise* noise = c.active_scene->getComponent<Component::Noise>(eid);
    if (noise == nullptr)
        return;
    
    Verse::Gui::draw_vec2(noise->offset.x, noise->offset.y, "offset", eid);
    ImGui::TableNextRow();
    
    Verse::Gui::draw_float(noise->freq, "freq", eid);
    ImGui::TableNextRow();
    Verse::Gui::draw_int(noise->levels, "levels", eid);
    ImGui::TableNextRow();
    Verse::Gui::draw_vec2(noise->dir.x, noise->dir.y, "dir", eid);
    ImGui::TableNextRow();
    
    Verse::Gui::draw_ui8(noise->fps, "fps", eid);
    ImGui::TableNextRow();
    
    Verse::Gui::draw_bool(noise->enabled, "enabled", eid);
#endif
}
