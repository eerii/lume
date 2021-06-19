//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_animation.h"
#include "log.h"

#include "gui.h"
#include "gui_types.h"

using namespace Verse;

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
    animation->curr_key = (entity["animation"]["curr_key"]) ? entity["animation"]["curr_key"].as<str>() : animation->frames.begin()->first;
    animation->size = (entity["animation"]["size"]) ? entity["animation"]["size"].as<int>() : 1;
}

void System::Animation::save(Component::Animation *anim, str path, std::vector<str> &key) {
    key[2] = "animation";
    
    key[3] = "frames";
    key.push_back(""); key.push_back("");
    for (std::pair<str, Component::AnimationFrame> f : anim->frames) {
        key[4] = f.first;
        
        key[5] = "index";
        Serialization::appendYAML(path, key, f.second.index, true);
        
        key[5] = "fps";
        Serialization::appendYAML(path, key, (int)round(1000.0f / (float)f.second.ms[0]), true);
    }
    key.pop_back(); key.pop_back();
    
    key[3] = "curr_key";
    Serialization::appendYAML(path, key, (str)anim->frames.begin()->first, true);
    
    key[3] = "size";
    Serialization::appendYAML(path, key, (int)anim->size, true);
}

void System::Animation::gui(Config &c, EntityID eid) {
#ifndef DISABLE_GUI
    
#endif
}
