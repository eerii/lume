//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_texture.h"

#include <glm/ext.hpp>

#include "ftime.h"
#include "s_animation.h"
#include "r_renderer.h"
#include "r_pipeline.h"
#include "r_textures.h"

#include "log.h"

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
}

void System::Texture::render(Config &c) {
    for (EntityID e : SceneView<Component::Texture>(*c.active_scene)) {
        Component::Texture* tex = c.active_scene->getComponent<Component::Texture>(e);
        Component::Animation* anim = c.active_scene->getComponent<Component::Animation>(e);
        Component::Noise* noise = c.active_scene->getComponent<Component::Noise>(e);
        
        if (tex->use_collider_transform) {
            Component::Collider* col = c.active_scene->getComponent<Component::Collider>(e);
            if (col != nullptr)
                tex->transform = col->transform;
            else
                tex->use_collider_transform = false;
        }
        
        if (tex->res.size() == 0)
            continue;
        
        Vec2 size = Vec2(1, (int)tex->layer.size());
        if (anim != nullptr) {
            System::Animation::update(c, e);
            size.x = anim->size;
        }
        
        for (int i = 0; i < size.y - ((noise != nullptr) ? 1 : 0); i++) {
            std::vector<float> vertices(v, v + sizeof v / sizeof v[0]);
            
            bool has_anim = (anim != nullptr and
                             anim->frames.size() > 0 and
                             anim->frames.count(anim->curr_key) > 0 and
                             anim->frames[anim->curr_key].index.size() > anim->curr_frame);
            
            for (int j = 0; j < 4; j++) {
                vertices[j*4+2] /= (float)size.x;
                vertices[j*4+3] = (vertices[j*4+3] + (float)i) / (float)size.y;
                
                if (has_anim)
                    vertices[j*4+2] += (float)anim->frames[anim->curr_key].index[anim->curr_frame] / (float)size.x;
            }
            
            if (tex->is_reversed) {
                vertices[0*4+2] = vertices[1*4+2];
                vertices[1*4+2] = vertices[2*4+2];
                vertices[2*4+2] = vertices[0*4+2];
                vertices[3*4+2] = vertices[1*4+2];
            }
            
            Rect2 dst = Rect2((tex->transform.pos() + tex->offset[i]), tex->transform.size());
            
            tex->data.model = Graphics::Renderer::matModel2D(dst.pos() - Vec2(BORDER_WIDTH, BORDER_WIDTH), dst.size());
            tex->data.vertices = vertices;
            tex->data.layer = tex->layer[i];
            
            Graphics::Renderer::renderTexture(c, tex->data);
        }
    }
}

void System::Texture::load(EntityID eid, YAML::Node &entity, Scene *s, Config &c) {
    Component::Texture* texture = s->addComponent<Component::Texture>(eid);
    if (not entity["texture"]["res"]) {
        log::error("You created a texture component for " + s->getName(eid) + " but it has no res for the texture");
        s->removeEntity(eid);
        return;
    }
    texture->res = entity["texture"]["res"].as<str>();
    Graphics::Texture::loadTexture(texture->res, texture);
    if (entity["texture"]["transform"] and entity["texture"]["transform"].IsSequence()) {
        texture->transform = entity["texture"]["transform"].as<Rect2<int>>();
    } else {
        Component::Collider* collider = s->getComponent<Component::Collider>(eid);
        if (collider == nullptr) {
            log::error("You didn't specity a position for " + s->getName(eid) + "'s texture and there is no collider");
            s->removeEntity(eid);
            return;
        }
        texture->transform = collider->transform;
        texture->use_collider_transform = true;
    }
    if (entity["texture"]["offset"]) {
        texture->offset = {};
        if (entity["texture"]["offset"].IsSequence()) {
            texture->offset = entity["texture"]["offset"].as<std::vector<Vec2<int>>>();
        } else {
            texture->offset.push_back(entity["texture"]["offset"].as<Vec2<int>>());
        }
    }
    if (entity["texture"]["layer"]) {
        texture->layer = {};
        if (entity["texture"]["layer"].IsSequence()) {
            texture->layer = entity["texture"]["layer"].as<std::vector<int>>();
        } else {
            texture->layer.push_back(entity["texture"]["layer"].as<int>());
        }
    }
}

void System::Texture::save(Component::Texture *tex, str path, std::vector<str> &key) {
    key[2] = "texture";
    
    key[3] = "res";
    Serialization::appendYAML(path, key, (str)tex->res, true);
    
    key[3] = "transform";
    if (tex->use_collider_transform)
        Serialization::appendYAML(path, key, (str)"collider", true);
    else
        Serialization::appendYAML(path, key, tex->transform, true);
    
    key[3] = "layer";
    if (tex->layer.size() == 1)
        Serialization::appendYAML(path, key, tex->layer[0], true);
    else
        Serialization::appendYAML(path, key, tex->layer, true);
    
    key[3] = "offset";
    if (tex->offset.size() == 1) {
        if (tex->offset[0] != Vec2(0,0))
            Serialization::appendYAML(path, key, tex->offset[0], true);
    } else {
        Serialization::appendYAML(path, key, tex->offset, true);
    }
}

void System::Texture::gui(Config &c, EntityID eid) {
#ifndef DISABLE_GUI
    Component::Texture* tex = c.active_scene->getComponent<Component::Texture>(eid);
    if (tex == nullptr)
        return;
    
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();
    ImGui::Text("res");
    
    ImGui::TableSetColumnIndex(1);
    float line_height = ImGui::GetStyle().FramePadding.y * 2.0f + ImGui::CalcTextSize("X").y;
    ImVec2 button_size = { line_height + 3.0f, line_height };
    if (ImGui::Button("L", button_size))
        Graphics::Texture::loadTexture(tex->res, tex);
    
    ImGui::SameLine();
    str res_label = "##res" + std::to_string(eid);
    ImGui::SetNextItemWidth(ImGui::GetColumnWidth());
    ImGui::InputText(res_label.c_str(), &tex->res);
    
    ImGui::TableNextRow();
    
    Verse::Gui::draw_bool(tex->use_collider_transform, "collider transform", eid);
    ImGui::TableNextRow();
    
    if (not tex->use_collider_transform) {
        Verse::Gui::draw_vec2(tex->transform.x, tex->transform.y, "pos", eid);
        ImGui::TableNextRow();
        Verse::Gui::draw_vec2(tex->transform.w, tex->transform.h, "size", eid);
        ImGui::TableNextRow();
    }
    
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();
    if (ImGui::TreeNode("layers")) {
        ImGui::TableNextRow();
        
        for (int i = 0; i < tex->offset.size(); i++) {
            if (tex->offset.size() > 1) {
                ImGui::TableSetColumnIndex(0);
                ImGui::AlignTextToFramePadding();
                str label = "layer " + std::to_string(i);
                ImGui::Text("%s", label.c_str());
                
                ImGui::SameLine();
                str button_label = "x##texlayer" + std::to_string(i) + std::to_string(eid);
                if (ImGui::SmallButton(button_label.c_str())) {
                    tex->offset.erase(tex->offset.begin() + i);
                    tex->layer.erase(tex->layer.begin() + i);
                }
                
                ImGui::TableNextRow();
            }
            
            str label = "offset " + std::to_string(i);
            Verse::Gui::draw_vec2(tex->offset[i].x, tex->offset[i].y, label, eid);
            ImGui::TableNextRow();
            
            Verse::Gui::draw_int(tex->layer[i], "layer " + std::to_string(i), eid);
            ImGui::TableNextRow();
        }
        
        ImGui::TableSetColumnIndex(0);
        ImGui::AlignTextToFramePadding();
        str button_label = "add layer##" + std::to_string(eid);
        if (ImGui::SmallButton(button_label.c_str())) {
            tex->offset.push_back(Vec2(0,0));
            tex->layer.push_back(0);
        }
        
        ImGui::TreePop();
    }
#endif
}
