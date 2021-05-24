//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "gui_menu.h"

#include "serialization.h"

#include "r_window.h"
#include "s_tilemap.h"
#include "s_scene_transition.h"

using namespace Verse;

#ifdef DEBUG
namespace {
    std::vector<str> available_scenes {
        "test_scene",
        "test_scene_2",
        "test_scene_3"
    };
}
#endif

void Gui::menu(Config &c) {
    ImGuiStyle& style = ImGui::GetStyle();
    
    if(ImGui::BeginMainMenuBar()) {
        //LUME
        //---------------------------
        if (ImGui::BeginMenu("[ lume ]")) {
            ImGui::Text("version: %s", c.version.c_str());
            ImGui::Text("res: %d x %d (x%d)", c.resolution.x, c.resolution.y, c.render_scale);
            
            str scene_name = "scene: " + ((c.active_scene != nullptr) ? c.active_scene->name : "NULL");
            if (ImGui::BeginMenu(scene_name.c_str())) {
#ifdef DEBUG
                //WARNING: Using this feature creates new scenes. Hopefully this should be managed, but it might be prone to errors.
                
                if (ImGui::BeginListBox(""))
                {
                    for (str name : available_scenes) {
                        const bool is_selected = (name == c.active_scene->name);
                        const char* scene_name = name.c_str();
                        
                        if (ImGui::Selectable(scene_name, is_selected)) {
                            Scene* s = new Scene();
                            Serialization::loadScene(name, s, c);
                            System::SceneTransition::handle(c, s, Vec2(30,0)); //TODO: Change for actual spawn point
                        }
                        
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndListBox();
                }
#endif
                ImGui::EndMenu();
            }
            
            if (c.active_camera == nullptr)
                ImGui::Text("camera: NULL");
            ImGui::EndMenu();
        }
        //---------------------------
        
        //OPTIONS
        //---------------------------
        if (ImGui::BeginMenu("opcións")) {
            ImGui::Text("game");
            ImGui::DragFloat("game speed", &c.game_speed, 0.1, 0.0, 4.0);
            
            ImGui::Text("");
            ImGui::Text("color");
            ImGui::SliderInt("palette", &c.palette_index, -1, c.num_palettes - 1);
            ImGui::Checkbox("use light", &c.use_light);
            ImGui::Checkbox("use grayscale", &c.use_grayscale);
            ImGui::EndMenu();
        }
        //---------------------------
        
        //FPS
        //---------------------------
        str fps_menu = "fps: " + std::to_string(c.fps);
        ImVec2 size = ImGui::CalcTextSize(fps_menu.c_str());
        size.x += style.FramePadding.x * 2 + style.ItemSpacing.x;
        
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(ImGui::GetIO().DisplaySize.x - size.x, 0));
        
        if (ImGui::BeginMenu(fps_menu.c_str())) {
            ImGui::Text("physics time: %d ms", c.physics_time);
            ImGui::Text("render time: %d ms", c.render_time);
            
            if (ImGui::Selectable("vsync", &c.use_vsync)) {
                Graphics::Window::updateVsync(c);
            }
            ImGui::EndMenu();
        }
        //---------------------------
        
        ImGui::EndMainMenuBar();
    }
}
