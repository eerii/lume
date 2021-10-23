//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include <variant>

#include "c_collider.h"
#include "c_circle_collider.h"
#include "c_actor.h"
#include "c_state.h"
#include "c_texture.h"
#include "c_animation.h"
#include "c_tilemap.h"
#include "c_text.h"
#include "c_camera.h"
#include "c_light.h"
#include "c_noise.h"
#include "c_timer.h"
#include "c_patrol.h"
#include "c_scene_transition.h"
#include "c_dialogue.h"
#include "c_player.h"

namespace Verse
{
    using ComponentType = std::variant<Component::Collider,
                                       Component::CircleCollider,
                                       Component::Actor,
                                       Component::State,
                                       Component::Texture,
                                       Component::Animation,
                                       Component::Tilemap,
                                       Component::Text,
                                       Component::Camera,
                                       Component::Light,
                                       Component::Noise,
                                       Component::Timer,
                                       Component::Patrol,
                                       Component::SceneTransition,
                                       Component::Dialogue,
                                       Component::Player>;

    static const std::vector<str> component_names = {
        "collider", "circle_collider",
        "actor", "state",
        "texture", "animation", "tilemap", "text", "camera",
        "light", "noise",
        "timer", "patrol", "scene_transition",
        "dialogue",
        "player"
    };
}

//TODO: Figure out if all of these are really necessary
//Try to make them automatic with reflection, maybe add a flag or something with the name and create a list automatically
