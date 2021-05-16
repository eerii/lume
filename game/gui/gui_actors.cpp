//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "gui_actors.h"

using namespace Verse;

void Gui::actors(Config &c) {
#ifdef ACTOR
#ifdef COLLIDER
    ImGui::Begin("Actors");
    
    static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_PadOuterX | ImGuiTableFlags_RowBg;
    
    if (ImGui::BeginTable("Position", 4, flags))
    {
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Position", ImGuiTableColumnFlags_WidthFixed, 96.0f);
        ImGui::TableSetupColumn("Velocity", ImGuiTableColumnFlags_WidthFixed, 128.0f);
        ImGui::TableSetupColumn("On Ground", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableHeadersRow();
        
        for (EntityID ent : SceneView<Component::Actor>(*c.active_scene))
        {
            Entity::EntityIndex index = Entity::getIndex(ent);
            
            ImGui::TableNextRow();
            
            ImGui::TableNextColumn();
            ImGui::Text("%s", c.active_scene->entity_names[index].c_str());
            
            ImGui::TableNextColumn();
            ImGui::Text("x: %d, y:%d",
                        (int)c.active_scene->getComponent<Component::Collider>(ent)->transform.x,
                        (int)c.active_scene->getComponent<Component::Collider>(ent)->transform.y);
            
            ImGui::TableNextColumn();
            ImGui::Text("x: %.2f, y:%.2f",
                        c.active_scene->getComponent<Component::Actor>(ent)->vel.x,
                        c.active_scene->getComponent<Component::Actor>(ent)->vel.y);
            
            ImGui::TableNextColumn();
            ImGui::Text("%s", c.active_scene->getComponent<Component::Actor>(ent)->is_on_ground ? "true" : "false");
        }
        ImGui::EndTable();
    }
    
    ImGui::End();
#endif
#endif
}
