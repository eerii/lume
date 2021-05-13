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

#define PHYSICS_UPDATE_SYSTEMS System::Actor::update(c, s); \
                               System::Camera::update(c, s);

#define RENDER_SYSTEMS System::Tilemap::render(scene, c); \
                       System::Texture::render(scene, c); \
                       System::Fire::render(scene, c); \
                       System::Light::render(scene, c); \
                       if (c.render_collision_boxes) \
                           System::Collider::render(scene, c); \
