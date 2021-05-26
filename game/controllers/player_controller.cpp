//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "player_controller.h"

#include "game.h"
#include "input.h"
#include "time.h"

#include "log.h"

using namespace Verse;
using namespace State;
using namespace Player;

namespace {
    int jump_impulse = 220;
    int min_jump_impulse = 70;

    Scene* scene;
    Component::Actor* actor;
    Component::Animation* anim;
    Component::Texture* tex;
    Component::Fire* fire;
    Component::Collider* collider;
    Component::Light* light;

    State::PlayerStates* state;

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

bool Controller::Player::controller(Config &c, EntityID eid, actor_move_func actor_move) {
    if (scene != c.active_scene or collider == nullptr or actor == nullptr or anim == nullptr or
        tex == nullptr or fire == nullptr or light == nullptr) {
        scene = c.active_scene;
        collider = c.active_scene->getComponent<Component::Collider>(eid);
        actor = c.active_scene->getComponent<Component::Actor>(eid);
        anim = c.active_scene->getComponent<Component::Animation>(eid);
        tex = c.active_scene->getComponent<Component::Texture>(eid);
        fire = c.active_scene->getComponent<Component::Fire>(eid);
        light = c.active_scene->getComponent<Component::Light>(eid);
    }
    
    if (state == nullptr)
        state = new PlayerStates(COYOTE_TIMEOUT, GRACE_TIMEOUT, 100, EPSILON); //TODO: CHANGE FOR ACTOR VALUES and propper epsilon
    
    //MOVE X
    if (Input::down(Input::Key::Left) or Input::down(Input::Key::A))
        move(c, false);
    if (Input::down(Input::Key::Right) or Input::down(Input::Key::D))
        move(c, true);
    
    if (not (Input::down(Input::Key::Left) or Input::down(Input::Key::A) or Input::down(Input::Key::Right) or Input::down(Input::Key::D)))
        state->move.handle(StopMovingEvent(actor->vel.x));
    
    if (state->move.is(IdleState()))
        actor->vel.x = 0;
    
    if (state->move.is(MovingState()))
        actor->vel.x = sign(actor->vel.x) * actor->max_move_speed;
    
    if (state->move.is(DeceleratingState()))
        actor->vel.x -= actor->friction_ground * c.physics_delta * sign(actor->vel.x); //TODO: Add friction air
        
    
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
        ui16 diff = (ui16)(Time::current - coyote_time);
        
        if (diff * c.game_speed > COYOTE_TIMEOUT) {
            previously_on_ground = false;
            coyote_time = 0;
        }
    }
    //Jump Grace
    if (jump_time != 0) {
        ui16 diff = (ui16)(Time::current - jump_time);
        
        if (actor->is_on_ground)
            jump();
        
        if (diff * c.game_speed > GRACE_TIMEOUT)
            jump_time = 0;
    }
    
    //Jump Animation
    if (not actor->is_on_ground) {
        if (on_jump or not previously_on_ground) {
            anim->curr_key = "jump_up";
        }
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
    
    //FALL OFF THE LEVEL
    if (collider->transform.y > 500) //TODO: Change
        respawn(c);
    
    bool moving = actor_move(c, eid, state);
    
    //ANIMATION
    if (state->move.is(IdleState())) {
        anim->curr_key = curr_idle_anim;
        flame_horizonal_offset = 0;
    }
    if (state->move.is(MovingState()) or state->move.is(AcceleratingState()) or state->move.is(DeceleratingState())) {
        anim->curr_key = "walk_1";
        flame_horizonal_offset = tex->is_reversed ? 1 : -1;
    }
    
    return moving;
}

void Controller::Player::move(Config &c, bool right) {
    actor->vel.x += actor->acc_ground * c.physics_delta * (right ? 1 : -1); //TODO: Add acc air
    
    tex->is_reversed = not right;
    
    state->move.handle(MoveEvent(right ? 1 : -1, actor->vel.x)); //TODO: Change direction state
}

void Controller::Player::jump() {
    actor->vel.y = -jump_impulse;
    jump_time = 0;
    on_jump = true;
    
    actor->is_on_ground = false;
    previously_on_ground = false;
}

void Controller::Player::respawn(Config &c) {
    auto closest_vec = [=](Vec2 v1, Vec2 v2) -> bool {
        return abs(v1.x - collider->transform.x) < abs(v2.x - collider->transform.x); };
    
    Vec2 closest_spawn = *std::min_element(c.active_scene->spawn.cbegin(), c.active_scene->spawn.cend(), closest_vec);
    
    collider->transform = closest_spawn;
    actor->vel = Vec2f(0,0);
    actor->is_on_ground = false;
}

str Controller::Player::getCurrentJumpState() {
    return (state == nullptr) ? "" : CURR_STATE(state->jump);
}

str Controller::Player::getCurrentMoveState() {
    return (state == nullptr) ? "" : CURR_STATE(state->move);
}
