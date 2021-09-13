//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "player_controller.h"

#include "game.h"
#include "input.h"
#include "ftime.h"
#include "log.h"

#include "s_collider.h"
#include "s_texture.h"
#include "s_camera.h"
#include "s_actor.h"

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
    Component::Collider* collider;
    Component::Light* light;

    Component::State* state;

    bool tried_jumping = false;
    bool previously_on_air = false;
    bool falling_tiny_bit = false;

    int light_strength = 150;
    ui32 light_timer = 0;
    str curr_idle_anim = "idle_1";

    float previous_game_speed = 1.0f;
}

bool Controller::Player::controller(Config &c, EntityID eid) {
    if (scene != c.active_scene or collider == nullptr or actor == nullptr or anim == nullptr or
        tex == nullptr or light == nullptr or state == nullptr) {
        scene = c.active_scene;
        collider = c.active_scene->getComponent<Component::Collider>(eid);
        actor = c.active_scene->getComponent<Component::Actor>(eid);
        anim = c.active_scene->getComponent<Component::Animation>(eid);
        tex = c.active_scene->getComponent<Component::Texture>(eid);
        light = c.active_scene->getComponent<Component::Light>(eid);
        state = c.active_scene->getComponent<Component::State>(eid);
    }
    
    if (state->states.size() == 0) {
        if (state == nullptr)
            log::error("Player has no state component");
        
        if (not state->has_state["jump"]) {
            state->states.push_back(State::Player::JumpSM(
                State::Player::FallingState(),
                State::Player::GroundedState(),
                State::Player::JumpingState(),
                State::Player::FallingCoyoteState(COYOTE_TIMEOUT),
                State::Player::FallingFromPlatformState(COYOTE_TIMEOUT),
                State::Player::FallingButJumpingState(GRACE_TIMEOUT),
                State::Player::FallingFasterState(),
                State::Player::FallingFasterButJumpingState(GRACE_TIMEOUT),
                State::Player::CrouchingState()
            ));
            
            state->has_state["jump"] = true;
            state->index["jump"] = state->states.size() - 1;
        }
        
        if (not state->has_state["move"]) {
            state->states.push_back(State::Player::MoveSM(
                State::Player::IdleState(),
                State::Player::AcceleratingState(100),
                State::Player::MovingState(),
                State::Player::DeceleratingState(EPSILON)
            ));
            
            state->has_state["move"] = true;
            state->index["move"] = state->states.size() - 1;
        }
    }
    
    if (previous_game_speed != c.game_speed)
        move_state.updateStates(IdleState(), AcceleratingState(100), MovingState(), DeceleratingState(EPSILON * c.game_speed)); //TODO: Change too
    
    
    //DOWN
    if (Input::down(Input::Key::S))
        down(c, eid);
    
    
    //GROUNDED
    if (jump_state.is(GroundedState())) {
        if (not checkGroundDown(c, eid))
            jump_state.handle(FallEvent());
    }
    
    
    //MOVE
    if (Input::down(Input::Key::Left) or Input::down(Input::Key::A))
        move(c, false);
    if (Input::down(Input::Key::Right) or Input::down(Input::Key::D))
        move(c, true);
    
    if (not (Input::down(Input::Key::Left) or Input::down(Input::Key::A) or Input::down(Input::Key::Right) or Input::down(Input::Key::D)))
        move_state.handle(StopMovingEvent(actor->vel.x));
    
    if (move_state.is(IdleState()))
        actor->vel.x = 0;
    
    if (move_state.is(MovingState()))
        actor->vel.x = sign(actor->vel.x) * actor->max_move_speed;
        
    
    //JUMP
    if (Input::pressed(Input::Key::Space))
        jump(c, eid);
    
    if (Input::released(Input::Key::Space))
        releaseJump();
    
    if (actor->vel.y > 0 and jump_state.is(JumpingState()))
        jump_state.handle(PeakJumpEvent());
    
    previously_on_air = not (jump_state.is(GroundedState()) or jump_state.is(CrouchingState()));
    falling_tiny_bit = (previously_on_air) ? falling_tiny_bit : false;
        
    jump_state.handle(TimeoutEvent(c.game_speed));
    tried_jumping = jump_state.is(FallingButJumpingState()) or jump_state.is(FallingFasterButJumpingState());
    
    
    //TEST CAMERA SHAKE
    if (Input::pressed(Input::Key::C))
        System::Camera::shake(c, 3000, 1.5f);
    
    
    //LIGHT
    if (light_timer == 0)
        light_timer = setTimer(500);
    
    if (checkTimer(light_timer)) {
        if (c.player_loses_light)
            light_strength--;
        light_timer = setTimer(500);
        curr_idle_anim = (light_strength > 75) ? "idle_1" : (light_strength > 50) ? "idle_2" : (light_strength > 25) ? "idle_3" : "idle_4";
    }
    
    if (light_strength < 0) {
        respawn(c);
    }
    
    light->radius = light_strength;
    
    
    //FALL OFF THE LEVEL
    if (*collider->transform.y > 500) //TODO: Change
        respawn(c);
    
    
    //ACTOR MOVE FUNCTION
    System::Actor::move(c, eid);
    bool on_ground = jump_state.is(GroundedState()) or jump_state.is(CrouchingState());
    
    
    //FRICTION
    {
        int friction = (on_ground) ? actor->friction_ground : actor->friction_air;
        if (move_state.is(DeceleratingState()))
            actor->vel.x -= friction * c.physics_delta * sign(actor->vel.x);
        
        int friction_extra = (on_ground) ? actor->friction_ground : actor->friction_extra;
        if (getMovingPlatformVelocity(c, eid).x != 0)
            friction_extra = 0;
        
        actor->extra_vel.x = (abs(actor->extra_vel.x) > 10) ?
                              actor->extra_vel.x - friction_extra * c.physics_delta * sign(actor->extra_vel.x) : 0;
        actor->extra_vel.y = (abs(actor->extra_vel.y) > 10) ?
                              actor->extra_vel.y - friction_extra * c.physics_delta * sign(actor->extra_vel.y) : 0;
    }
    
    
    //JUMP GRACE
    if (tried_jumping and jump_state.is(JumpingState())) {
        actor->vel.y = -jump_impulse;
        falling_tiny_bit = false;
        anim->queue.push_back("jump_end_short");
        anim->queue.push_back("jump_start_short");
    }
    //TODO: Check if jump released for this :)
        
    
    //ANIMATION
    if (move_state.is(IdleState()) or move_state.is(DeceleratingState())) {
        anim->target_key = curr_idle_anim;
    }
    if (move_state.is(MovingState()) or move_state.is(AcceleratingState())) {
        anim->target_key = "walk_1";
        if (move_state.is(FallingCoyoteState()))
            falling_tiny_bit = checkGroundDown(c, eid, TINY_BIT);
    }
    
    if (jump_state.is(JumpingState()))
        anim->target_key = "jump_up";
    if (not falling_tiny_bit and (jump_state.is(FallingState()) or jump_state.is(FallingCoyoteState()) or jump_state.is(FallingButJumpingState())))
        anim->target_key = "jump_down";
    if (not falling_tiny_bit and previously_on_air and on_ground) //TODO: Platform check
        anim->queue.push_back("jump_end");
    
    return true;
}

void Controller::Player::move(Config &c, bool right) {
    actor->vel.x += actor->acc_ground * c.physics_delta * (right ? 1 : -1); //TODO: Add acc air
    
    tex->is_reversed = not right;
    
    move_state.handle(MoveEvent(right ? 1 : -1, actor->vel.x));
}

void Controller::Player::jump(Config &c, EntityID eid) {
    if (jump_state.is(JumpingState()))
        return;
    
    jump_state.handle(JumpEvent());
    
    if (jump_state.is(JumpingState())) {
        actor->vel.y = -jump_impulse;
        falling_tiny_bit = false;
        anim->queue.push_back("jump_start");
        
        actor->extra_vel.x += getMovingPlatformVelocity(c, eid).x * (1.0f + 0.5f * abs(actor->vel.x / actor->max_move_speed));
        move_state.handle(MoveEvent(sign(actor->extra_vel.x), actor->extra_vel.x));
    }
}

void Controller::Player::releaseJump() {
    if (not jump_state.is(JumpingState()))
        return;
    
    jump_state.handle(ReleaseJumpEvent());
    
    if (jump_state.is(FallingState()) and -actor->vel.y > min_jump_impulse)
        actor->vel.y = -min_jump_impulse;
}

void Controller::Player::down(Config &c, EntityID eid) {
    collider->transform += Vec2::j;
    System::Collider::CollisionInfo collisions = System::Collider::checkCollisions(c, eid);
    collider->transform -= Vec2::j;
    for (System::Collider::CollisionInfoPair collision : collisions) {
        if (collision.second[System::Collider::Layers::Platform]) {
            jump_state.handle(DownEvent(true));
        }
    }
}

void Controller::Player::respawn(Config &c) {
    auto closest_vec = [=](Vec2 v1, Vec2 v2) -> bool {
        return abs(v1.x - *collider->transform.x) < abs(v2.x - *collider->transform.x); };
    
    Vec2 closest_checkpoint = *std::min_element(c.active_scene->checkpoints.cbegin(), c.active_scene->checkpoints.cend(), closest_vec);
    collider->transform = closest_checkpoint;
    
    actor->vel = Vec2f(0,0);
    jump_state.handle(FallEvent());
    
    light_strength = 150;
}

bool Controller::Player::checkGroundDown(Config &c, EntityID eid, int down) {
    collider->transform += Vec2::j * down;
    ui8 colliding = System::Actor::collisions(c, eid, false);
    collider->transform -= Vec2::j * down;
    
    return colliding == System::Actor::Colliding::Solid;
}

Vec2f Controller::Player::getMovingPlatformVelocity(Config &c, EntityID eid) {
    Vec2f vel = Vec2f(0,0);
    
    collider->transform += Vec2::j;
    
    System::Collider::CollisionInfo collision_info = System::Collider::checkCollisions(c, eid);
    for (System::Collider::CollisionInfoPair collision : collision_info) {
        if (collision.second[System::Collider::Layers::Platform] or collision.second[System::Collider::Layers::SolidPlatform]) {
            Component::Collider* platform_collider = c.active_scene->getComponent<Component::Collider>(collision.first);
            Component::Collider* actor_collider = c.active_scene->getComponent<Component::Collider>(eid);
            
            bool above = *actor_collider->transform.y + *actor_collider->transform.h <= *platform_collider->transform.y + 1;
            if (above) {
                Component::Actor* platform_actor = c.active_scene->getComponent<Component::Actor>(collision.first);
                if (platform_actor != nullptr)
                    vel = platform_actor->vel;
            }
        }
    }
    
    collider->transform -= Vec2::j;
    
    return vel;
}
