//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "player_controller.h"

#include "game.h"
#include "input.h"
#include "time.h"
#include "log.h"

#include "s_collider.h"

#define TINY_BIT 8

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

    bool tried_jumping = false;
    bool falling_tiny_bit = false;

    int flame_offsets[18] = { 0, 0, 0, 1, 0, 1, -1, 1, 1, 1, 1, -1, 0, 0, 1, 1, 1, 2 };
    int flame_horizonal_offset = 0;
    Vec2 flame_initial_offset = Vec2(-1, -1);

    int light_strength = 100;
    ui32 light_time = 0;
    str curr_idle_anim = "idle_1";

    float previous_game_speed = 1.0f;
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
        resetState(c); //TODO: CHANGE FOR ACTOR VALUES and propper epsilon and change in delete state pls
    if (previous_game_speed != c.game_speed)
        state->move.updateStates(IdleState(), AcceleratingState(100), MovingState(), DeceleratingState(EPSILON * c.game_speed)); //TODO: Change too
    
    //MOVE
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
    if (Input::pressed(Input::Key::Space))
        jump();
    
    if (Input::released(Input::Key::Space))
        releaseJump();
    
    if (actor->vel.y > 0 and state->jump.is(JumpingState()))
        state->jump.handle(PeakJumpEvent());
    
    if (state->jump.is(GroundedState()) or state->jump.is(CrouchingState()))
        falling_tiny_bit = false;
        
    state->jump.handle(TimeoutEvent(c.game_speed));
    tried_jumping = state->jump.is(FallingButJumpingState()) or state->jump.is(FallingFasterButJumpingState());
    
    
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
    
    
    //ACTOR MOVE FUNCTION
    bool moving = actor_move(c, eid, state);
    
    
    //JUMP GRACE
    if (tried_jumping and state->jump.is(JumpingState()))
        actor->vel.y = -jump_impulse;
    //TODO: Check if jump released for this :)
        
    
    //ANIMATION
    if (state->move.is(IdleState())) {
        anim->curr_key = curr_idle_anim;
        flame_horizonal_offset = 0;
    }
    if (state->move.is(MovingState()) or state->move.is(AcceleratingState()) or state->move.is(DeceleratingState())) {
        anim->curr_key = "walk_1";
        flame_horizonal_offset = tex->is_reversed ? 1 : -1;
        
        if (state->jump.is(FallingCoyoteState()))
            falling_tiny_bit = checkGroundDown(c, eid, TINY_BIT);
    }
    if (state->jump.is(JumpingState()))
        anim->curr_key = "jump_up";
    if (not falling_tiny_bit and (state->jump.is(FallingState()) or state->jump.is(FallingCoyoteState()) or state->jump.is(FallingButJumpingState())))
        anim->curr_key = "jump_down";
    
    return moving;
}

void Controller::Player::move(Config &c, bool right) {
    actor->vel.x += actor->acc_ground * c.physics_delta * (right ? 1 : -1); //TODO: Add acc air
    
    tex->is_reversed = not right;
    
    state->move.handle(MoveEvent(right ? 1 : -1, actor->vel.x));
}

void Controller::Player::jump() {
    if (state->jump.is(JumpingState()))
        return;
    
    state->jump.handle(JumpEvent());
    
    if (state->jump.is(JumpingState()))
        actor->vel.y = -jump_impulse;
}

void Controller::Player::releaseJump() {
    if (not state->jump.is(JumpingState()))
        return;
    
    state->jump.handle(ReleaseJumpEvent());
    
    if (state->jump.is(FallingState()) and -actor->vel.y > min_jump_impulse)
        actor->vel.y = -min_jump_impulse;
}

void Controller::Player::respawn(Config &c) {
    auto closest_vec = [=](Vec2 v1, Vec2 v2) -> bool {
        return abs(v1.x - collider->transform.x) < abs(v2.x - collider->transform.x); };
    
    Vec2 closest_spawn = *std::min_element(c.active_scene->spawn.cbegin(), c.active_scene->spawn.cend(), closest_vec);
    
    collider->transform = closest_spawn;
    actor->vel = Vec2f(0,0);
    state->jump.handle(FallEvent());
}

bool Controller::Player::checkGroundDown(Config &c, EntityID eid, int down) {
    bool is_ground_down = false;
    
    collider->transform += Vec2::j * down;
    
    std::bitset<MAX_COLLISION_LAYERS> collision_layers = System::Collider::checkCollisions(c, eid);
    if (collision_layers.any())
        is_ground_down = true;
    
    collider->transform -= Vec2::j * down;
    
    return is_ground_down;
}

str Controller::Player::getCurrentJumpState() {
    return (state == nullptr) ? "" : CURR_STATE(state->jump);
}

str Controller::Player::getCurrentMoveState() {
    return (state == nullptr) ? "" : CURR_STATE(state->move);
}

void Controller::Player::resetState(Config &c) {
    delete state;
    state = new PlayerStates(COYOTE_TIMEOUT, GRACE_TIMEOUT, 100, EPSILON * c.game_speed); //TODO: Change
}
