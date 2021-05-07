//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include "log.h"

#include "scene.h"
#include "config.h"

#include "system_list.h"
#include "r_textures.h"

#include "player_controller.h"
#include "free_controller.h"

namespace Verse
{
    void init(Scene &scene, Config &config) {
        EntityID test = scene.createEntity("Player");
        
        Component::Collider* collider = scene.addComponent<Component::Collider>(test);
        collider->transform = Rect2(32, 64, 9, 14);
        
        Component::Texture* texture = scene.addComponent<Component::Texture>(test);
        Graphics::Texture::loadTexture("res/graphics/lume.png", texture);
        texture->transform = Rect2(32, 64, 11, 14);
        texture->offset = Vec2(-1, 0);
        
        Component::Animation* animation = scene.addComponent<Component::Animation>(test);
        animation->frames["idle"] = {0, 5, 2};
        animation->frames["walk"] = {7,8,9,10,11,12};
        animation->frames["jump_down"] = {15,16};
        animation->curr_key = "idle";
        animation->size = 26; //calculate_size
        
        Component::Actor* actor = scene.addComponent<Component::Actor>(test);
        actor->controller = [&scene, test]() -> bool {return Controller::Player::controller(scene, test);};
        actor->max_move_speed = 100;
        actor->max_fall_speed = 250;
        actor->acc_ground = 1000;
        actor->friction_ground = 0.25;
        
        Component::Light* light = scene.addComponent<Component::Light>(test);
        light->pos = Vec2(3, 7);
        light->radius = 144;
        
        Component::Camera* camera = scene.addComponent<Component::Camera>(test);
        camera->pos = Vec2f(0,0);
        camera->bounds = Rect2(scene.size * 0.5f, scene.size);
        System::Camera::init(camera);
        System::Camera::setActive(camera);
    }
}
