//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include <variant>

//Base
#include "c_collider.h"
#include "c_actor.h"
#include "c_state.h"
#include "c_texture.h"
#include "c_animation.h"
#include "c_tilemap.h"
#include "c_text.h"
#include "c_camera.h"
#include "c_light.h"
#include "c_timer.h"
#include "c_patrol.h"
#include "c_scene_transition.h"

//Extra
#include "c_fire.h"
#include "c_player.h"

#define USE_C_FIRE
#define USE_C_PLAYER

namespace Verse
{
    using ComponentType = std::variant<Component::Collider,
                                       Component::Actor,
                                       Component::State,
                                       Component::Texture,
                                       Component::Animation,
                                       Component::Tilemap,
                                       Component::Text,
                                       Component::Camera,
                                       Component::Light,
                                       Component::Timer,
                                       Component::Patrol,
                                       Component::SceneTransition,
                                       Component::Fire,
                                       Component::Player>;

    static const std::vector<str> component_names = {
        "collider", "actor", "state",
        "texture", "animation", "tilemap", "text", "camera",
        "light",
        "timer", "patrol", "scene_transition",
        "fire", "player"
    };
}
