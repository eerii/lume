//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "c_collider.h"
#include "c_texture.h"
#include "c_actor.h"
#include "c_tilemap.h"
#include "c_light.h"
#include "c_camera.h"

#define COMPONENTS logComponentID(Collider); \
                   logComponentID(Texture); \
                   logComponentID(Actor); \
                   logComponentID(Tilemap); \
                   logComponentID(Light); \
                   logComponentID(Camera); \

#define COLLIDER
#define TEXTURE
#define ACTOR
#define TILEMAP
#define LIGHT
