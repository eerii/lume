//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_patrol.h"

#include "gui.h"
#include "gui_types.h"

using namespace Verse;

void System::Patrol::load(EntityID eid, YAML::Node &entity, Scene *s, Config &c) {
    Component::Patrol* patrol = s->addComponent<Component::Patrol>(eid);
    if (entity["patrol"]["points"]) {
        if (entity["patrol"]["points"].IsSequence())
            patrol->points = entity["patrol"]["points"].as<std::vector<Vec2<int>>>();
        if (entity["patrol"]["points"].IsScalar())
            patrol->points = {entity["patrol"]["points"].as<Vec2<int>>()};
    }
}

//TODO: Patrol save

void System::Patrol::gui(Config &c, EntityID eid) {
#ifndef DISABLE_GUI
    Component::Patrol* patrol = c.active_scene->getComponent<Component::Patrol>(eid);
    if (patrol == nullptr)
        return;
    
    for (int i = 0; i < patrol->points.size(); i++) {
        str label = "point " + std::to_string(i);
        
        ImGuiStyle& style = ImGui::GetStyle();
        
        ImGui::PushID(label.c_str());
        
        float line_height = style.FramePadding.y * 2.0f + ImGui::CalcTextSize("X").y;
        ImVec2 button_size = { line_height + 3.0f, line_height };
        
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("%s", label.c_str());
        
        ImGui::SameLine();
        str button_label = "x##patrol" + std::to_string(i) + std::to_string(eid);
        if (ImGui::SmallButton(button_label.c_str())) {
            patrol->points.erase(patrol->points.begin() + i);
        }
        
        ImGui::TableSetColumnIndex(1);
        
        if (ImGui::Button("X", button_size))
            patrol->points[i].x = 0;

        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetColumnWidth() * 0.5f - button_size.x);
        str x_label = "##X" + std::to_string(eid);
        ImGui::DragInt(x_label.c_str(), &patrol->points[i].x);
        
        ImGui::SameLine();
        if (ImGui::Button("Y", button_size))
            patrol->points[i].y = 0;

        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetColumnWidth());
        str y_label = "##Y" + std::to_string(eid);
        ImGui::DragInt(y_label.c_str(), &patrol->points[i].y);
        
        ImGui::PopID();
        
        ImGui::TableNextRow();
    }
    
    ImGui::TableSetColumnIndex(0);
    if (ImGui::SmallButton("add point")) {
        patrol->points.push_back((patrol->points.size() > 0) ? patrol->points[0] : Vec2(0,0));
    }
#endif
}
