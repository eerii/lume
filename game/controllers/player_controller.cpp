//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "player_controller.h"
#include "game.h"
#include "input.h"
#include "time.h"

#define EPSILON 5

using namespace Verse;

namespace {
    int jump_impulse = 220;
    int min_jump_impulse = 70;
    int jump_grace = 150;
    int jump_coyote = 100;

    Component::Actor* actor;
    Component::Animation* anim;
    Component::Texture* tex;
    Component::Fire* fire;
    Component::Collider* collider;
    Component::Light* light;

    ui64 jump_time = 0;
    ui64 coyote_time = 0;
    bool on_jump = false;
    bool previously_on_ground = false;

    int flame_offsets[18] = { 0, 0, 0, 1, 0, 1, -1, 1, 1, 1, 1, -1, 0, 0, 1, 1, 1, 2 };
    int flame_horizonal_offset = 0;
    Vec2 flame_initial_offset = Vec2(-1, -1);

    int light_strength = 100;
    ui32 light_time = 0;
    str curr_idle_anim = "idle_1";
}

bool Controller::Player::controller(Scene &s, Config &c, EntityID eid) {
    if (collider == nullptr or actor == nullptr or anim == nullptr or tex == nullptr or fire == nullptr or light == nullptr) {
        collider = s.getComponent<Component::Collider>(eid);
        actor = s.getComponent<Component::Actor>(eid);
        anim = s.getComponent<Component::Animation>(eid);
        tex = s.getComponent<Component::Texture>(eid);
        fire = s.getComponent<Component::Fire>(eid);
        light = s.getComponent<Component::Light>(eid);
    }
    
    //MOVE X
    if (Input::down(Input::Key::Left) or Input::down(Input::Key::A)) {
        actor->vel.x -= actor->acc_ground * DELTA * c.game_speed;
        tex->is_reversed = true;
        flame_horizonal_offset = 1;
    }
    if (Input::down(Input::Key::Right) or Input::down(Input::Key::D)) {
        actor->vel.x += actor->acc_ground * DELTA * c.game_speed;
        tex->is_reversed = false;
        flame_horizonal_offset = -1;
    }
    
    if (Input::down(Input::Key::Left) or Input::down(Input::Key::A) or Input::down(Input::Key::Right) or Input::down(Input::Key::D)) {
        anim->curr_key = "walk_1";
    } else {
        anim->curr_key = curr_idle_anim;
        flame_horizonal_offset = 0;
    }
    
    if (abs(actor->vel.x) > actor->max_move_speed)
        actor->vel.x = sign(actor->vel.x) * actor->max_move_speed;
    
    if (!Input::down(Input::Key::Left) && !Input::down(Input::Key::Right)) {
        if (abs(actor->vel.x) > EPSILON * c.game_speed)
            actor->vel.x -= sign(actor->vel.x) * (1 / actor->friction_ground) * actor->max_move_speed * DELTA * c.game_speed;
        else
            actor->vel.x = 0;
    }
    
    //JUMP
    if (Input::pressed(Input::Key::Space)) {
        if (actor->is_on_ground or previously_on_ground) {
            jump();
        } else {
            jump_time = Time::current;
        }
    }
    //Stop Jump
    if (Input::released(Input::Key::Space) and on_jump) {
        on_jump = false;
        
        if (-actor->vel.y > min_jump_impulse)
            actor->vel.y = -min_jump_impulse;
    }
    //Coyote Time
    if (actor->is_on_ground) {
        previously_on_ground = true;
        on_jump = false;
    }
    if (!actor->is_on_ground and previously_on_ground and coyote_time == 0 and !on_jump)
        coyote_time = Time::current;
    
    if (coyote_time != 0) {
        int diff = (int)(Time::current - coyote_time);
        
        if (diff > jump_coyote) {
            previously_on_ground = false;
            coyote_time = 0;
        }
    }
    //Jump Grace
    if (jump_time != 0) {
        int diff = (int)(Time::current - jump_time);
        
        if (actor->is_on_ground)
            jump();
        
        if (diff > jump_grace)
            jump_time = 0;
    }
    
    //Jump Animation
    if (not actor->is_on_ground) {
        anim->curr_key = "jump_up";
    }
    
    //FLAME
    if (anim->frames[anim->curr_key].size() > anim->curr_frame)
        fire->offset = flame_initial_offset - Vec2(flame_horizonal_offset, flame_offsets[anim->frames[anim->curr_key][anim->curr_frame]]);
    else
        fire->offset = flame_initial_offset;
    
    //LIGHT
    light->radius = light_strength;
    if (light_time > 500) {
        light_time = 0;
        //TODO: ENABLE light_strength -= 1;
        curr_idle_anim = (light_strength > 75) ? "idle_1" : (light_strength > 50) ? "idle_2" : (light_strength > 25) ? "idle_3" : "idle_4";
    } else {
        light_time += Time::delta * c.game_speed;
    }
    
    if (light_strength == 0) {
        light_strength = 100;
    }
    
    //RESPAWN
    if (collider->transform.y > 500) {
        collider->transform = Vec2(32, 64); //TODO: Change for a propper spawn
    }
    
    return actor->vel != Vec2f(0,0);
}

void Controller::Player::jump() {
    actor->vel.y = -jump_impulse;
    jump_time = 0;
    on_jump = true;
    
    actor->is_on_ground = false;
    previously_on_ground = false;
}
