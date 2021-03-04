//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "r_window.h"
#include "imgui.h"

using namespace Verse;

void Window::onResize(SDL_Event &e, Config &c) {
    if (((float)e.window.data1 / (float)e.window.data2) >= ((float)c.resolution.x / (float)c.resolution.y)) {
        //Longer in the X direction
        c.render_scale = floor((float)e.window.data2 / (float)c.resolution.y);
        
    } else {
        //Longer in the Y direction
        c.render_scale = floor((float)e.window.data1 / (float)c.resolution.x);
    }
    
    c.window_size = Vec2(e.window.data1, e.window.data2);
    
    float w = c.render_scale * c.resolution.x;
    float h = c.render_scale * c.resolution.y;
    float padx = (float)(c.window_size.x - w) / 2.0f;
    float pady = (float)(c.window_size.y - h) / 2.0f;
    c.window_padding = Vec2(padx, pady);
    
    ImGuiIO& imgui_io = ImGui::GetIO();
    imgui_io.DisplaySize.x = static_cast<float>(e.window.data1);
    imgui_io.DisplaySize.y = static_cast<float>(e.window.data2);
}
