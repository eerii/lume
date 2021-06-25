//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_camera.h"

#include <glm/ext.hpp>

#include "game.h"
#include "ftime.h"
#include "log.h"
#include "r_renderer.h"
#include "controller_list.h"

#include "gui.h"
#include "gui_types.h"

using namespace Verse;

namespace {
    Vec2 pixel_perfect_move;
    Vec2f extra_move;

    Vec2f shake_vec;
    ui32 shake_timer = 0;
    float shake_strength;
}

void System::Camera::init(Component::Camera* camera) {
    camera->vel = Vec2f(0,0);
}

void System::Camera::update(Config &c) {
    for (EntityID e : SceneView<Component::Camera>(*c.active_scene)) {
        Component::Camera* cam = c.active_scene->getComponent<Component::Camera>(e);
        if (cam != c.active_camera)
            continue;
        
        cam->controller();
        
        Vec2f total = cam->remainder + cam->vel * c.physics_delta;
        Vec2 to_move = Vec2(floor(total.x), floor(total.y));
        cam->pos += to_move;
        cam->remainder = total - to_move.to_float();
    }
}

void System::Camera::prerender(Config &c) {
    for (EntityID e : SceneView<Component::Camera>(*c.active_scene)) {
        Component::Camera* cam = c.active_scene->getComponent<Component::Camera>(e);
        if (cam != c.active_camera)
            continue;
        
        pixel_perfect_move = Vec2(0.5f * c.resolution.x - cam->pos.x, 0.5f * c.resolution.y - cam->pos.y);
        extra_move = (c.use_subpixel_cam) ? Vec2f(-cam->remainder.x, cam->remainder.y) * c.render_scale : Vec2f(0,0);
        
        if(checkTimer(shake_timer)) {
            shake_vec = Vec2f(0,0);
            shake_timer = 0;
            shake_strength = 0;
        }
        
        if(shake_timer != 0)
            shake_vec = (Vec2f(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX) * 2.0f - Vec2f::one) *
                         c.render_scale * shake_strength - extra_move;
        
        cam->m_pixel = glm::translate(glm::mat4(1.0f), glm::vec3(pixel_perfect_move.x, pixel_perfect_move.y, 0.0f));
        cam->m_extra = glm::translate(glm::mat4(1.0f), glm::vec3(extra_move.x + shake_vec.x, extra_move.y + shake_vec.y, 0.0f));
    }
}

void System::Camera::shake(Config &c, ui16 ms, float strength) {
    shake_timer = setTimer(ms);
    shake_strength = strength;
    if (strength > c.render_scale * BORDER_WIDTH) {
        log::warn("You tried to use camera shake with a higher shake than the border width. Please adjust one of them.");
        shake_strength = c.render_scale * BORDER_WIDTH;
    }
}

void System::Camera::load(EntityID eid, YAML::Node &entity, Scene *s, Config &c) {
    Component::Camera* camera = s->addComponent<Component::Camera>(eid);
    if (entity["camera"]["pos"])
        camera->pos = entity["camera"]["pos"].as<Vec2>();
    camera->bounds = Rect2(0,0,0,0);
    if (entity["camera"]["bounds"]) {
        if (entity["camera"]["bounds"].IsSequence()) {
            camera->bounds = entity["camera"]["bounds"].as<Rect2>();
        } else {
            if (entity["camera"]["bounds"].as<str>() == "scene")
                camera->bounds = Rect2(s->size * 0.5f, s->size);
            if (entity["camera"]["bounds"].as<str>() == "none")
                camera->bounds = Rect2(0,0,0,0);
        }
    }
    if (entity["camera"]["controller"]) {
        str name = entity["camera"]["controller"].as<str>();
        if (camera_controllers.find(name) != camera_controllers.end())
            camera->controller = [&c, eid, name](){ return camera_controllers[name](c, eid); };
        else
            log::error("Error getting camera controller for " + c.active_scene->getName(eid));
        camera->current_controller = name;
    }
    System::Camera::init(camera);
}

void System::Camera::save(Component::Camera *cam, str path, std::vector<str> &key, Scene *s) {
    key[2] = "camera";
    
    key[3] = "controller";
    Serialization::appendYAML(path, key, (str)cam->current_controller, true);
    
    if (cam->pos != Vec2(0,0)) {
        key[3] = "pos";
        Serialization::appendYAML(path, key, cam->pos, true);
    }
    
    key[3] = "bounds";
    if (cam->bounds.size == s->size) {
        Serialization::appendYAML(path, key, (str)"scene", true);
    } else if (cam->bounds.size == Vec2(0,0)) {
        Serialization::appendYAML(path, key, (str)"none", true);
    } else {
        Serialization::appendYAML(path, key, cam->bounds, true);
    }
}

void System::Camera::gui(Config &c, EntityID eid) {
#ifndef DISABLE_GUI
    Component::Camera* cam = c.active_scene->getComponent<Component::Camera>(eid);
    if (cam == nullptr)
        return;
    
    Verse::Gui::draw_vec2(cam->target_pos.x, cam->target_pos.y, "target pos", eid);
    ImGui::TableNextRow();
    
    Verse::Gui::draw_vec2(cam->pos.x, cam->pos.y, "pos", eid);
    ImGui::TableNextRow();
    
    Verse::Gui::draw_vec2(cam->remainder.x, cam->remainder.y, "remainder", eid);
    ImGui::TableNextRow();
    
    Verse::Gui::draw_vec2(cam->vel.x, cam->vel.y, "vel", eid);
    ImGui::TableNextRow();
    
    Verse::Gui::draw_vec2(cam->bounds.pos.x, cam->bounds.pos.y, "bounds pos", eid);
    ImGui::TableNextRow();
    Verse::Gui::draw_vec2(cam->bounds.size.x, cam->bounds.size.y, "bounds size", eid);
    ImGui::TableNextRow();
    
    
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("controller");
    
    ImGui::TableSetColumnIndex(1);
    str controller_label = "##controller" + std::to_string(eid);
    ImGui::SetNextItemWidth(ImGui::GetColumnWidth());
    
    std::vector<str> available_controllers;
    ui8 current_controller_index = 0;
    ui8 i = 0;
    for (std::pair<str, std::function<bool(Config &c, EntityID eid)>> ct : camera_controllers) {
        available_controllers.push_back(ct.first);
        if (cam->current_controller == ct.first)
            current_controller_index = i;
        i++;
    }
    
    ui8 prev_controller_index = current_controller_index;
    ImGui::Combo(controller_label.c_str(), current_controller_index, available_controllers);
    
    if (prev_controller_index != current_controller_index) {
        str name = available_controllers[current_controller_index];
        cam->controller = [&c, eid, name](){
            return camera_controllers[name](c, eid);
        };
        cam->current_controller = name;
    }
    
    bool active = c.active_camera == cam;
    if (not active) {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::AlignTextToFramePadding();
        str button_label = "make active##" + std::to_string(eid);
        if (ImGui::SmallButton(button_label.c_str())) {
            c.active_camera = cam;
        }
    }
#endif
}
