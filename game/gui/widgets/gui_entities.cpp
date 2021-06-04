//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "gui_entities.h"
#include "gui_types.h"
#include "r_textures.h"

#include "log.h"

using namespace Verse;

namespace {
    ui32 n = 0;
    std::map<str, std::function<void(Config&, EntityID)>> c_funcs;
}

void components(Config &c, Signature mask, EntityID e);

void Gui::entities(Config &c) {
    ImGui::Begin("entities");
    
    ImGui::Text("scene: %s", ((c.active_scene != nullptr) ? c.active_scene->name.c_str() : "NULL"));
    ImGui::Text("number of entities: %d", n);
    
    static ImGuiTableFlags flags = ImGuiTableFlags_PadOuterX | ImGuiTableFlags_RowBg;
    
    if (ImGui::BeginTable("inspector", 2, flags))
    {
        ImGui::TableSetupColumn("entity", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("property", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();
        
        n = 0;
        for (EntityID e : SceneView<>(*c.active_scene))
        {
            ImGui::PushID(e);
            Entity::EntityIndex index = Entity::getIndex(e);
            n++;
            
            ImGui::TableNextRow();
            
            //Entity Name
            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            str entity_name = c.active_scene->entity_names[index];
            bool node_open = ImGui::TreeNode(entity_name.c_str());
            
            //Properties
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("id: [%d]", index);
            
            ImGui::SameLine();
            str add_label = "add##" + std::to_string(e);
            if (ImGui::SmallButton(add_label.c_str())) {
                
            }
            
            ImGui::SameLine();
            str remove_label = "remove##" + std::to_string(e);
            if (ImGui::SmallButton(remove_label.c_str())) {
                c.active_scene->removeEntity(e);
            }
            
            //Submenu
            if (node_open) {
                Signature mask = c.active_scene->mask[index];
                
                components(c, mask, e);
                
                ImGui::TreePop();
            }
            
            ImGui::PopID();
        }
        ImGui::EndTable();
    }
    
    ImGui::End();
}

void c_collider(Config &c, EntityID e) {
    Component::Collider* col = c.active_scene->getComponent<Component::Collider>(e);
    
    Verse::Gui::draw_vec2(col->transform.x, col->transform.y, "pos");
    ImGui::TableNextRow();
    Verse::Gui::draw_vec2(col->transform.w, col->transform.h, "size");
    ImGui::TableNextRow();
    
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("layer");
    
    ImGui::TableSetColumnIndex(1);
    str layer_label = "##collayer" + std::to_string(e);
    ImGui::SetNextItemWidth(ImGui::GetColumnWidth());
    ImGui::Combo(layer_label.c_str(), col->layer, Component::c_layers_name);
}

void c_texture(Config &c, EntityID e) {
    Component::Texture* tex = c.active_scene->getComponent<Component::Texture>(e);
    
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("res");
    
    ImGui::TableSetColumnIndex(1);
    
    float line_height = ImGui::GetStyle().FramePadding.y * 2.0f + ImGui::CalcTextSize("X").y;
    ImVec2 button_size = { line_height + 3.0f, line_height };
    if (ImGui::Button("L", button_size))
        Graphics::Texture::loadTexture(tex->res, tex);
    
    ImGui::SameLine();
    str res_label = "##res" + std::to_string(e);
    ImGui::SetNextItemWidth(ImGui::GetColumnWidth());
    ImGui::InputText(res_label.c_str(), &tex->res);
    
    ImGui::TableNextRow();
    
    Verse::Gui::draw_vec2(tex->transform.x, tex->transform.y, "pos");
    ImGui::TableNextRow();
    Verse::Gui::draw_vec2(tex->transform.w, tex->transform.h, "size");
    ImGui::TableNextRow();
    
    for (int i = 0; i < tex->offset.size(); i++) {
        str label = "offset " + std::to_string(i);
        Verse::Gui::draw_vec2(tex->offset[i].x, tex->offset[i].y, label);
        ImGui::TableNextRow();
        
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("layer %d", i);
        
        ImGui::TableSetColumnIndex(1);
        str layer_label = "##layer" + std::to_string(e) + std::to_string(i);
        ImGui::SetNextItemWidth(ImGui::GetColumnWidth());
        ImGui::DragInt(layer_label.c_str(), &tex->layer[i]);
        ImGui::TableNextRow();
    }
    
}

void c_animation(Config &c, EntityID e) {
    //Component::Animation* anim = c.active_scene->getComponent<Component::Animation>(e);
}

void c_tilemap(Config &c, EntityID e) {
    //Component::Tilemap* tile = c.active_scene->getComponent<Component::Tilemap>(e);
}

void c_actor(Config &c, EntityID e) {
    //Component::Actor* actor = c.active_scene->getComponent<Component::Actor>(e);
}

void c_light(Config &c, EntityID e) {
    //Component::Light* light = c.active_scene->getComponent<Component::Light>(e);
}

void c_camera(Config &c, EntityID e) {
    //Component::Camera* cam = c.active_scene->getComponent<Component::Camera>(e);
}

void c_fire(Config &c, EntityID e) {
    //Component::Fire* fire = c.active_scene->getComponent<Component::Fire>(e);
}

void c_scene_transition(Config &c, EntityID e) {
    //Component::SceneTransition* trans = c.active_scene->getComponent<Component::SceneTransition>(e);
}

void c_player(Config &c, EntityID e) {
    //Component::Player* player = c.active_scene->getComponent<Component::Player>(e);
}

void components(Config &c, Signature mask, EntityID e) {
    if (c_funcs.size() == 0) {
        c_funcs["collider"] = c_collider;
        c_funcs["texture"] = c_texture;
        c_funcs["animation"] = c_animation;
        c_funcs["tilemap"] = c_tilemap;
        c_funcs["actor"] = c_actor;
        c_funcs["light"] = c_light;
        c_funcs["camera"] = c_camera;
        c_funcs["fire"] = c_fire;
        c_funcs["sceneTransition"] = c_collider;
        c_funcs["player"] = c_player;
    }
    
    for (int i = 0; i < MAX_COMPONENTS; i++) {
        if (mask[i] == 0)
            continue;
        
        ImGui::PushID(i);
        ImGui::TableNextRow();
        
        ImGui::TableSetColumnIndex(0);
        ImGui::AlignTextToFramePadding();
        str component_name = Component::getName(i);
        component_name[0] = std::tolower(component_name[0]);
        bool node_open = ImGui::TreeNode(component_name.c_str());
        
        ImGui::TableSetColumnIndex(1);
        str remove_component_label = "remove##" + std::to_string(e) + std::to_string(i);
        if (ImGui::SmallButton(remove_component_label.c_str())) {
            c.active_scene->removeComponent(e, i);
        }
        
        if (node_open) {
            ImGui::TableNextRow();
            
            c_funcs[component_name](c, e);
            
            ImGui::TreePop();
        }
        
        ImGui::PopID();
    }
}
