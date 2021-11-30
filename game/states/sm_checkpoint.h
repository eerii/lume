//project fresa, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include <functional>

#include "state_machine.h"
#include "static_str.h"
#include "scene.h"

#include "c_light.h"
#include "c_animation.h"
#include "c_noise.h"
#include "c_texture.h"

#define MAKE_STRING(TYPE) [[maybe_unused]] static constexpr auto make_string(Types<TYPE>) { return Str{#TYPE}; };

namespace Fresa::State::Checkpoint
{
    struct ActivateEvent {
        Scene* s;
        EntityID eid;
        ActivateEvent(Scene* p_s, EntityID p_eid) : s(p_s), eid(p_eid) {};
    };
    struct DeactivateEvent {};

    struct DeactivatedState;
    struct ActivatedState;

    struct DeactivatedState : Default<Nothing> {
        using Default::handle;
        
        void onEnter(const DeactivateEvent &e) {
            
        }
        
        To<ActivatedState> handle(const ActivateEvent &e) { return To<ActivatedState>(); };
    };

    struct ActivatedState : Default<Nothing> {
        using Default::handle;
        
        void onEnter(const ActivateEvent &e) {
            Component::Texture* tex = e.s->getComponent<Component::Texture>(e.eid);
            tex->use_collider_transform = false;
            
            Component::Collider* col = e.s->getComponent<Component::Collider>(e.eid);
            e.s->checkpoints.push_back(col->transform.pos());
            col->transform += Rect2(-24, -16, 48, 32);
            
            Component::Light* light = e.s->getComponent<Component::Light>(e.eid);
            light->radius = 75;
            
            Component::Animation* anim = e.s->getComponent<Component::Animation>(e.eid);
            if (anim != nullptr)
                anim->target_key = "active";
            
            Component::Noise* noise = e.s->getComponent<Component::Noise>(e.eid);
            if (noise != nullptr)
                noise->enabled = true;
        }
        
        To<DeactivatedState> handle(const DeactivateEvent &e) { return To<DeactivatedState>(); };
    };

    MAKE_STRING(ActivateEvent);
    MAKE_STRING(DeactivateEvent);
    MAKE_STRING(ActivatedState);
    MAKE_STRING(DeactivatedState);

    using CheckpointSM = StateMachine<DeactivatedState, ActivatedState>;
}
