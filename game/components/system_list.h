//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "component_list.h"

#include "s_collider.h"
#include "s_actor.h"
#include "s_state.h"
#include "s_texture.h"
#include "s_animation.h"
#include "s_tilemap.h"
#include "s_text.h"
#include "s_camera.h"
#include "s_light.h"
#include "s_timer.h"
#include "s_patrol.h"
#include "s_scene_transition.h"
#include "s_noise.h"
#include "s_dialogue.h"
#include "s_player.h"

namespace Verse::System
{
    static inline void physicsUpdateSystems(Config &c) {
        Actor::update(c);
        Noise::update(c);
        Camera::update(c);
    };

    static inline void renderUpdateSystems(Config &c) {
        Camera::prerender(c);
        Tilemap::render(c);
        if (not c.tme_active) {
            Texture::render(c);
            Text::render(c);
            Noise::render(c);
        }
    };
}
