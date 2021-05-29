//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "gui_entities.h"

using namespace Verse;

void Gui::entities(Config &c) {
    ImGui::Begin("Entity Manager");
    
    static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_PadOuterX | ImGuiTableFlags_RowBg;
    
    if (ImGui::BeginTable("Entities", 3, flags))
    {
        ImGui::TableSetupColumn("Index", ImGuiTableColumnFlags_WidthFixed, 64.0f);
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Signature", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableHeadersRow();
        
        for (EntityID ent : SceneView<>(*c.active_scene))
        {
            Entity::EntityIndex index = Entity::getIndex(ent);
            
            ImGui::TableNextRow();
            
            ImGui::TableNextColumn();
            ImGui::Text("%d", index);
            
            ImGui::TableNextColumn();
            ImGui::Text("%s", c.active_scene->entity_names[index].c_str());
            
            ImGui::TableNextColumn();
            ImGui::Text("%s", c.active_scene->mask[index].to_string().c_str());
        }
        ImGui::EndTable();
    }
    
    ImGui::End();
}