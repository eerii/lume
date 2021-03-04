//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "gui.h"
#include "input.h"

#include "gui_entities.h"
#include "gui_actors.h"
#include "gui_performance.h"

using namespace Verse;

void Gui::update(float delta, Config &c) {
    ImGuiIO& imgui_io = ImGui::GetIO();
    
    imgui_io.DeltaTime = delta;
    
    Vec2 mouse_pos = Vec2(Input::mouse().x, Input::mouse().y);
    if (c.use_shaders)
        mouse_pos -= c.window_padding;
    
    imgui_io.MousePos = ImVec2(mouse_pos.x, mouse_pos.y);
    imgui_io.MouseDown[0] = Input::down(SDL_BUTTON_LEFT);
    imgui_io.MouseDown[1] = Input::down(SDL_BUTTON_RIGHT);
    imgui_io.MouseWheel = static_cast<float>(Input::mouseWheel());
}

void Gui::prerender(Scene &scene, Config &c, ui16 &fps) {
#ifdef USE_OPENGL
    
#else
    ImGui::NewFrame();
    
    Gui::entities(scene, c);
    Gui::actors(scene);
    Gui::performance(fps);
#endif
}

void Gui::render() {
#ifdef USE_OPENGL
    
#else
    ImGui::Render();
    ImGuiSDL::Render(ImGui::GetDrawData());
#endif
}
