//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "s_collider.h"
#include "s_texture.h"
#include "s_actor.h"
#include "s_tilemap.h"
#include "s_light.h"
#include "s_camera.h"
#include "s_fire.h"

#include "component_list.h"

#define PHYSICS_UPDATE_SYSTEMS System::Actor::update(c); \
                               System::Camera::update(c);

#define RENDER_SYSTEMS System::Camera::update(c); \
                       System::Tilemap::render(c); \
                       if (not c.tme_active) { \
                           System::Texture::render(c); \
                           System::Fire::render(c); \
                       }
                       
