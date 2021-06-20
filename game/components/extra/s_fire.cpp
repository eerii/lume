//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_fire.h"

#include "ftime.h"

#include "r_pipeline.h"
#include "r_textures.h"
#include "r_renderer.h"

#include "gui.h"
#include "gui_types.h"

using namespace Verse;

namespace {
    float refresh_rate = 0;
}

void System::Fire::init(Component::Fire *fire) {
    Vec2 fixed_size = fire->transform.size;
    if (fixed_size.x % 4 != 0)
        fixed_size.x = fixed_size.x - fixed_size.x % 4 + 4;
    if (fixed_size.y % 4 != 0)
        fixed_size.y = fixed_size.y - fixed_size.y % 4 + 4;
    
#ifndef __EMSCRIPTEN__
    fire->p_data = (ui8*)malloc(fixed_size.x * fixed_size.y * sizeof(ui8));
#else
    fire->p_data = (ui8*)malloc((*fire->transform.w+1) * (*fire->transform.h+1) * 4);
#endif
    
    Graphics::Texture::createGradient(*fire->transform.w, fire->g_tex);
}

void System::Fire::render(Config &c) {
    if (refresh_rate == 0)
        refresh_rate = Graphics::getRefreshRate();
    
    for (EntityID e : SceneView<Component::Fire>(*c.active_scene)) {
        Component::Fire* fire = c.active_scene->getComponent<Component::Fire>(e);
        
        fire->noise_time += ((float)Time::delta / 1000.0f) * c.game_speed;
        if (fire->noise_time > (1.0f / (float)fire->fps)) {
            fire->noise_offset++;
            Graphics::Texture::createPerlinNoise(fire->transform.size,
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

void System::Fire::gui(Config &c, EntityID eid) {
#ifndef DISABLE_GUI
    Component::Fire* fire = c.active_scene->getComponent<Component::Fire>(eid);
    
    Verse::Gui::draw_vec2(*fire->transform.x, *fire->transform.y, "pos", eid);
    ImGui::TableNextRow();
    Verse::Gui::draw_vec2(*fire->transform.w, *fire->transform.h, "size", eid, [fire](){ init(fire); });
    ImGui::TableNextRow();
    Verse::Gui::draw_vec2(fire->offset.x, fire->offset.y, "offset", eid);
    ImGui::TableNextRow();
    
    Verse::Gui::draw_float(fire->freq, "freq", eid);
    ImGui::TableNextRow();
    Verse::Gui::draw_int(fire->levels, "levels", eid);
    ImGui::TableNextRow();
    Verse::Gui::draw_vec2(fire->dir.x, fire->dir.y, "dir", eid);
    ImGui::TableNextRow();
    
    Verse::Gui::draw_int(fire->layer, "layer", eid);
    ImGui::TableNextRow();
    Verse::Gui::draw_ui8(fire->fps, "fps", eid);
    ImGui::TableNextRow();
    
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("texture");
    
    ImGui::TableSetColumnIndex(1);
    float line_height = ImGui::GetStyle().FramePadding.y * 2.0f + ImGui::CalcTextSize("X").y;
    ImVec2 button_size = { line_height + 3.0f, line_height };
    if (ImGui::Button("L", button_size))
        Graphics::Texture::loadTexture(fire->flame_tex_res, fire->flame_tex);
    
    ImGui::SameLine();
    str res_label = "##res" + std::to_string(eid);
    ImGui::SetNextItemWidth(ImGui::GetColumnWidth());
    ImGui::InputText(res_label.c_str(), &fire->flame_tex_res);
#endif
}
