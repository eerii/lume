//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "gui_player.h"

#include "player_controller.h"

using namespace Verse;

void Gui::player(Config &c) {
#ifdef ACTOR
#ifdef COLLIDER
    ImGui::Begin("Player");
    
    static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_PadOuterX | ImGuiTableFlags_RowBg;
    
    if (ImGui::BeginTable("Position", 4, flags))
    {
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Position", ImGuiTableColumnFlags_WidthFixed, 96.0f);
        ImGui::TableSetupColumn("Velocity", ImGuiTableColumnFlags_WidthFixed, 128.0f);
        ImGui::TableSetupColumn("On Ground", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableHeadersRow();
        
        for (EntityID e : SceneView<Component::Player>(*c.active_scene))
        {
            ImGui::TableNextRow();
            
            ImGui::TableNextColumn();
            ImGui::Text("%s", c.active_scene->getName(e).c_str());
            
            ImGui::TableNextColumn();
            ImGui::Text("x: %d, y:%d",
                        (int)c.active_scene->getComponent<Component::Collider>(e)->transform.x,
                        (int)c.active_scene->getComponent<Component::Collider>(e)->transform.y);
            
            ImGui::TableNextColumn();
            ImGui::Text("x: %.2f, y:%.2f",
                        c.active_scene->getComponent<Component::Actor>(e)->vel.x,
                        c.active_scene->getComponent<Component::Actor>(e)->vel.y);
            
            ImGui::TableNextColumn();
            //ImGui::Text("%s", c.active_scene->getComponent<Component::Actor>(e)->is_on_ground ? "true" : "false");
        }
        ImGui::EndTable();
        
        ImGui::Text("Move State: %s", Controller::Player::getCurrentMoveState().c_str());
        ImGui::Text("Jump State: %s", Controller::Player::getCurrentJumpState().c_str());
    }
    
    ImGui::End();
#endif
#endif
}
