//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_animation.h"
#include "log.h"

#include "gui.h"
#include "gui_types.h"

using namespace Verse;

namespace {
    std::vector<str> new_frame;
}

void System::Animation::update(Config &c, EntityID eid) {
    Component::Animation* anim = c.active_scene->getComponent<Component::Animation>(eid);
    
    if (anim->timer == 0)
        anim->timer = setTimer(anim->frames[anim->target_key].ms[anim->curr_frame]);
    
    if (anim->curr_key == "")
        anim->curr_key = anim->target_key;
    
    if (checkTimer(anim->timer, c.game_speed) or (anim->queue_left == 0 and anim->queue.size() > 0)) {
        anim->queue_left = (ui16)anim->queue.size();
        
        anim->curr_frame = (anim->curr_frame < anim->frames[anim->target_key].index.size() - 1) ? anim->curr_frame + 1 : 0;
        anim->curr_key = anim->target_key;
        
        if (anim->queue.size() > 0) {
            anim->curr_key = anim->queue[0];
            anim->queue.erase(anim->queue.begin());
            anim->target_key = anim->curr_key;
            anim->curr_frame = 0;
        }
        
        anim->timer = setTimer(anim->frames[anim->curr_key].ms[anim->curr_frame]);
    }
    
    if (anim->queue_left == 0 and anim->curr_key != anim->target_key) {
        anim->curr_key = anim->target_key;
        anim->curr_frame = 0;
        anim->change_now = false;
        stopTimer(anim->timer);
        anim->timer = setTimer(anim->frames[anim->curr_key].ms[anim->curr_frame]);
    }
    
    //log::info("%s %s %d %d %d", curr_key.c_str(), anim->curr_key.c_str(), time(), anim->queue.size(), queue_left);
}

void System::Animation::load(EntityID eid, YAML::Node &entity, Scene *s, Config &c) {
    Component::Animation* animation = s->addComponent<Component::Animation>(eid);
    ui16 default_fps = (entity["animation"]["default_fps"]) ? entity["animation"]["default_fps"].as<ui16>() : 3;
    if (not (entity["animation"]["frames"] and entity["animation"]["frames"].IsMap())) {
        log::error("You created an animation component for " + s->getName(eid) + " but it has no frames");
        s->removeEntity(eid);
        return;
    }
    for(YAML::const_iterator i=entity["animation"]["frames"].begin(); i != entity["animation"]["frames"].end(); i++) {
        str animation_name = i->first.as<str>();
        
        if (i->second.IsSequence()) {
            animation->frames[animation_name].index = i->second.as<std::vector<ui16>>();
            
            std::vector<ui16> ms(animation->frames[animation_name].index.size(), (ui16)round(1000.0f / (float)default_fps));
            animation->frames[animation_name].ms = ms;
            
            continue;
        }
        
        if (i->second.IsMap()) {
            if (not (i->second["index"] and i->second["index"].IsSequence()))
                continue;
            animation->frames[animation_name].index = i->second["index"].as<std::vector<ui16>>();
            
            if (not i->second["fps"]) {
                std::vector<ui16> ms(animation->frames[animation_name].index.size(), (ui16)round(1000.0f / (float)default_fps));
                animation->frames[animation_name].ms = ms;
                continue;
            }
            if (i->second["fps"].IsScalar()) {
                std::vector<ui16> ms(animation->frames[animation_name].index.size(), (ui16)round(1000.0f / i->second["fps"].as<float>()));
                animation->frames[animation_name].ms = ms;
                continue;
            }
            std::vector<ui16> ms = i->second["fps"].as<std::vector<ui16>>();
            std::transform(ms.begin(), ms.end(), ms.begin(), [](ui16 &c){ return (ui16)round(1000.0f / (float)c); });
            animation->frames[animation_name].ms = ms;
        }
    }
    animation->target_key = (entity["animation"]["curr_key"]) ? entity["animation"]["curr_key"].as<str>() : animation->frames.begin()->first;
    animation->size = (entity["animation"]["size"]) ? entity["animation"]["size"].as<int>() : 1;
}

void System::Animation::save(Component::Animation *anim, str path, std::vector<str> &key) {
    key[2] = "animation";
    
    key[3] = "frames";
    for (std::pair<str, Component::AnimationFrame> f : anim->frames) {
        std::vector<str> new_key = {key[0], key[1], key[2], key[3], f.first, ""};
        
        new_key[5] = "fps";
        Serialization::appendYAML(path, new_key, (int)round(1000.0f / (float)f.second.ms[0]), true);
        
        new_key[5] = "index";
        Serialization::appendYAML(path, new_key, f.second.index, true);
    }
    
    key[3] = "curr_key";
    Serialization::appendYAML(path, key, (str)anim->frames.begin()->first, true);
    
    key[3] = "size";
    Serialization::appendYAML(path, key, (int)anim->size, true);
}

void System::Animation::gui(Config &c, EntityID eid) {
#ifndef DISABLE_GUI
    Component::Animation* anim = c.active_scene->getComponent<Component::Animation>(eid);
    if (anim == nullptr)
        return;
    
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();
    ImGui::Text("current key");
    ImGui::TableSetColumnIndex(1);
    ImGui::Text("%s", anim->curr_key.c_str());
    ImGui::TableNextRow();
    
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();
    ImGui::Text("current frame");
    ImGui::TableSetColumnIndex(1);
    ImGui::Text("%d", anim->curr_frame);
    ImGui::TableNextRow();
    
    int j = 0;
    new_frame.resize(anim->frames.size(), "");
    for (auto [key, frame] : anim->frames) {
        ImGui::TableSetColumnIndex(0);
        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s", key.c_str());
        
        ImGui::TableSetColumnIndex(1);
        for (int i = 0; i < frame.index.size(); i++) {
            if (ImGui::SmallButton(std::to_string(frame.index[i]).c_str())) {
                anim->frames[key].index.erase(anim->frames[key].index.begin() + i);
                anim->frames[key].ms.erase(anim->frames[key].ms.begin() + i);
            }
            ImGui::SameLine();
        }
        
        str label = "##newframe" + std::to_string(j);
        ImGui::PushItemWidth(20);
        ImGui::InputText(label.c_str(), &new_frame[j]);
        ImGui::SameLine();
        if (ImGui::SmallButton("+")) {
            anim->frames[key].index.push_back(std::stoi(new_frame[j]));
            anim->frames[key].ms.push_back(anim->frames[key].ms[0]);
            new_frame[j] = "";
        }
        ImGui::TableNextRow();
        
        j++;
    }
    
    //TODO: Add animation FPS
    //TODO: Add new animation names
#endif
}
