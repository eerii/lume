//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

//Base
#include "c_collider.h"
#include "c_actor.h"
#include "c_state.h"
#include "c_texture.h"
#include "c_animation.h"
#include "c_tilemap.h"
#include "c_camera.h"
#include "c_light.h"
#include "c_timer.h"
#include "c_patrol.h"
#include "c_scene_transition.h"

//Extra
#include "c_fire.h"
#include "c_player.h"

#define COMPONENTS logComponentID(Collider); \
                   logComponentID(Actor); \
                   logComponentID(State); \
                   logComponentID(Texture); \
                   logComponentID(Animation); \
                   logComponentID(Tilemap); \
                   logComponentID(Camera); \
                   logComponentID(Light); \
                   logComponentID(Timer); \
                   logComponentID(Patrol); \
                   logComponentID(SceneTransition); \
                   logComponentID(Fire); \
                   logComponentID(Player); \

#define USE_C_FIRE
#define USE_C_PLAYER
