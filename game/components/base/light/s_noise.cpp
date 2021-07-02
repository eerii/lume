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
    
    noise->size = tex->size;
    noise->noise_data = std::vector<ui8>(noise->size.x * noise->size.y);
    
    Math::perlinNoise(noise->size, Vec2(0,0), noise->freq, noise->levels, noise->noise_data.data(), true);
    noise->noise_tex = Graphics::Texture::createTexture(noise->noise_data.data(), noise->size.x, noise->size.y, false);
}

void System::Noise::update(Config &c) {
    for (EntityID e : SceneView<Component::Noise>(*c.active_scene)) {
        Component::Noise* noise = c.active_scene->getComponent<Component::Noise>(e);
        Component::Texture* tex = c.active_scene->getComponent<Component::Texture>(e);
        
        if (not noise->enabled)
            continue;
        
        if (noise->size != tex->size) {
            noise->size = tex->size;
            noise->noise_data = std::vector<ui8>(noise->size.x * noise->size.y);
        }
        
        noise->noise_time += c.physics_delta;
        if (noise->fps != 0 and noise->noise_time > (1.0f / (float)noise->fps)) {
            noise->noise_offset++;
            Graphics::Texture::createPerlinNoise(noise->size, noise->dir * noise->noise_offset, noise->freq,
                                                 noise->levels, noise->noise_data.data(), noise->noise_tex);
            
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
        glm::mat4 tex_vertices = glm::transpose(glm::make_mat4x4(v));
        tex_vertices[2] /= (float)tex_size.x;
        tex_vertices[3] = (tex_vertices[3] + (float)i) / (float)tex_size.y;
        
        if (anim != nullptr and
            anim->frames.size() > 0 and
            anim->frames.count(anim->curr_key) > 0 and
            anim->frames[anim->curr_key].index.size() > anim->curr_frame)
            tex_vertices[2] += (float)anim->frames[anim->curr_key].index[anim->curr_frame] / (float)tex_size.x;
        
        if (tex->is_reversed) {
            tex_vertices[2][0] = tex_vertices[2][1];
            tex_vertices[2][1] = tex_vertices[2][2];
            tex_vertices[2][2] = tex_vertices[2][0];
            tex_vertices[2][3] = tex_vertices[2][1];
        }
        
        tex_vertices = glm::transpose(tex_vertices);
        
        Rect2 dst = Rect2 (tex->render_pos + noise->offset + ((tex->offset.size() > i) ? tex->offset[i] : Vec2(0, 0)), noise->size);
        glm::mat4 model = Graphics::Renderer::matModel2D(dst.pos - Vec2(BORDER_WIDTH, BORDER_WIDTH), dst.size);
        
        Graphics::Renderer::renderNoise(c, noise->noise_tex, tex->tex_id, model, glm::value_ptr(tex_vertices), vn, tex->layer[i]);
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
#endif
}
