//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "gui_menu.h"
#include "imgui.h"

using namespace Verse;

void Gui::menu(Config &c, int fps) {
    ImGuiStyle& style = ImGui::GetStyle();
    
    if(ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("[ lume ]")) {
            if (ImGui::MenuItem("test")) {
                log::info("hi");
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("opcións")) {
            if (ImGui::DragFloat("game speed", &c.game_speed, 0.1, 0.0, 4.0)) {
                
            }
            if (ImGui::MenuItem("test")) {
                log::info("hi");
            }
            ImGui::EndMenu();
        }
        
        //FPS
        //---------------------------
        str fps_menu = "fps: " + std::to_string(fps);
        ImVec2 size = ImGui::CalcTextSize(fps_menu.c_str());
        size.x += style.FramePadding.x * 2 + style.ItemSpacing.x;
        
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(ImGui::GetIO().DisplaySize.x - size.x, 0));
        
        if (ImGui::BeginMenu(fps_menu.c_str())) {
            ImGui::EndMenu();
        }
        //---------------------------
        
        ImGui::EndMainMenuBar();
    }
}
