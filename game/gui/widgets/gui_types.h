//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "gui.h"

namespace Verse::Gui
{

static void draw_vec2(int &x, int &y, str label, float reset = 0.0f) {
    ImGuiStyle& style = ImGui::GetStyle();
    
    ImGui::PushID(label.c_str());
    
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("%s", label.c_str());
    
    float line_height = style.FramePadding.y * 2.0f + ImGui::CalcTextSize("X").y;
    ImVec2 button_size = { line_height + 3.0f, line_height };
    float width = (ImGui::GetColumnWidth() - button_size.x) * 0.5f - 1.5f;
    
    ImGui::TableSetColumnIndex(1);
    
    if (ImGui::Button("X", button_size))
        x = reset;

    ImGui::SameLine();
    ImGui::SetNextItemWidth(width);
    ImGui::DragInt("##X", &x);
    ImGui::SameLine();

    if (ImGui::Button("Y", button_size))
        y = reset;

    ImGui::SameLine();
    ImGui::SetNextItemWidth(width);
    ImGui::DragInt("##Y", &y);
    ImGui::SameLine();
    
    ImGui::PopID();
}

}
