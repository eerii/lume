//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_animation.h"
#include "log.h"

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
