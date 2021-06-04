//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "gui.h"

namespace Verse::Gui
{

static void draw_vec2(int &x, int &y, str label, int eid, float reset = 0.0f) {
    ImGuiStyle& style = ImGui::GetStyle();
    
    ImGui::PushID(label.c_str());
    
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("%s", label.c_str());
    
    float line_height = style.FramePadding.y * 2.0f + ImGui::CalcTextSize("X").y;
    ImVec2 button_size = { line_height + 3.0f, line_height };
    
    ImGui::TableSetColumnIndex(1);
    
    if (ImGui::Button("X", button_size))
        x = reset;

    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetColumnWidth() * 0.5f - button_size.x);
    str x_label = "##X" + std::to_string(eid);
    ImGui::DragInt(x_label.c_str(), &x);
    ImGui::SameLine();

    if (ImGui::Button("Y", button_size))
        y = reset;

    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetColumnWidth());
    str y_label = "##Y" + std::to_string(eid);
    ImGui::DragInt(y_label.c_str(), &y);
    ImGui::SameLine();
    
    ImGui::PopID();
}

static void draw_vec2(float &x, float &y, str label, int eid, float reset = 0.0f) {
    ImGuiStyle& style = ImGui::GetStyle();
    
    ImGui::PushID(label.c_str());
    
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("%s", label.c_str());
    
    float line_height = style.FramePadding.y * 2.0f + ImGui::CalcTextSize("X").y;
    ImVec2 button_size = { line_height + 3.0f, line_height };
    
    ImGui::TableSetColumnIndex(1);
    
    if (ImGui::Button("X", button_size))
        x = reset;

    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetColumnWidth() * 0.5f - button_size.x);
    str x_label = "##X" + std::to_string(eid);
    ImGui::DragFloat(x_label.c_str(), &x, 0.1f, -100000, 100000, "%.1f");
    ImGui::SameLine();

    if (ImGui::Button("Y", button_size))
        y = reset;

    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetColumnWidth());
    str y_label = "##Y" + std::to_string(eid);
    ImGui::DragFloat(y_label.c_str(), &y, 0.1f, -100000, 100000, "%.1f");
    ImGui::SameLine();
    
    ImGui::PopID();
}

static void draw_int(int &i, str label, int eid) {
    ImGui::PushID(label.c_str());
    
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("%s", label.c_str());
    
    ImGui::TableSetColumnIndex(1);
    str i_label = "##" + label + std::to_string(eid);
    ImGui::SetNextItemWidth(ImGui::GetColumnWidth());
    ImGui::DragInt(i_label.c_str(), &i);
    
    ImGui::PopID();
}

static void draw_float(float &f, str label, int eid) {
    ImGui::PushID(label.c_str());
    
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("%s", label.c_str());
    
    ImGui::TableSetColumnIndex(1);
    str f_label = "##" + label + std::to_string(eid);
    ImGui::SetNextItemWidth(ImGui::GetColumnWidth());
    ImGui::DragFloat(f_label.c_str(), &f, 0.1f, -100000, 100000, "%.1f");
    
    ImGui::PopID();
}

static void draw_bool(bool &b, str label, int eid) {
    ImGui::PushID(label.c_str());
    
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("%s", label.c_str());
    
    ImGui::TableSetColumnIndex(1);
    str i_label = "##" + label + std::to_string(eid);
    ImGui::Checkbox(i_label.c_str(), &b);
    
    ImGui::PopID();
}

}
