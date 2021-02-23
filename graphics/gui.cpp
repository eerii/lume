//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "gui.h"
#include "input.h"

#include "gui_entities.h"
#include "gui_actors.h"

using namespace Verse;

void Gui::update(float delta) {
    ImGuiIO& imgui_io = ImGui::GetIO();
    
    imgui_io.DeltaTime = delta;
    
    imgui_io.MousePos = ImVec2(static_cast<float>(Input::mouse().x), static_cast<float>(Input::mouse().y));
    imgui_io.MouseDown[0] = Input::down(SDL_BUTTON_LEFT);
    imgui_io.MouseDown[1] = Input::down(SDL_BUTTON_RIGHT);
    imgui_io.MouseWheel = static_cast<float>(Input::mouse_wheel());
}

void Gui::prerender(Scene &scene, Config &c) {
    ImGui::NewFrame();
    
    Gui::entities(scene, c);
    Gui::actors(scene);
    
    //ImGui::ShowDemoWindow();
}

void Gui::render() {
    ImGui::Render();
    ImGuiSDL::Render(ImGui::GetDrawData());
}
