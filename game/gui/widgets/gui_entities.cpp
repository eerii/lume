//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "gui_entities.h"

#include "log.h"

using namespace Verse;

namespace {
    ui32 n = 0;
}

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
            str select_label = "select##" + std::to_string(e);
            if (ImGui::SmallButton(select_label.c_str())) {
                
            }
            ImGui::SameLine();
            str remove_label = "remove##" + std::to_string(e);
            if (ImGui::SmallButton(remove_label.c_str())) {
                c.active_scene->removeEntity(e);
            }
            
            //Submenu
            if(node_open) {
                Signature mask = c.active_scene->mask[index];
                for (int i = 0; i < MAX_COMPONENTS; i++) {
                    if (mask[i] == 0)
                        continue;
                    
                    ImGui::PushID(i);
                    ImGui::TableNextRow();
                    
                    ImGui::TableSetColumnIndex(0);
                    ImGui::AlignTextToFramePadding();
                    str component_name = Component::getName(i);
                    ImGui::Text("%s", component_name.c_str());
                    ImGui::SameLine();
                    str remove_component_label = "x##" + std::to_string(e) + std::to_string(i);
                    if (ImGui::SmallButton(remove_component_label.c_str())) {
                        c.active_scene->removeComponent(e, i);
                    }
                    
                    ImGui::TableSetColumnIndex(1);
                    
                    ImGui::PopID();
                }
                ImGui::TreePop();
            }
            
            ImGui::PopID();
        }
        ImGui::EndTable();
    }
    
    ImGui::End();
}
