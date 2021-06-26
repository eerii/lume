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

    Component::State* c_state;
    State::PlayerStates * state;

    bool tried_jumping = false;
    bool previously_on_air = false;
    bool falling_tiny_bit = false;

    int light_strength = 100;
    ui32 light_timer = 0;
    str curr_idle_anim = "idle_1";

    float previous_game_speed = 1.0f;
}

bool Controller::Player::controller(Config &c, EntityID eid) {
    if (scene != c.active_scene or collider == nullptr or actor == nullptr or anim == nullptr or
        tex == nullptr or light == nullptr or c_state == nullptr) {
        scene = c.active_scene;
        collider = c.active_scene->getComponent<Component::Collider>(eid);
        actor = c.active_scene->getComponent<Component::Actor>(eid);
        anim = c.active_scene->getComponent<Component::Animation>(eid);
        tex = c.active_scene->getComponent<Component::Texture>(eid);
        light = c.active_scene->getComponent<Component::Light>(eid);
        c_state = c.active_scene->getComponent<Component::State>(eid);
    }
    
    if (state == nullptr)
        resetState(c, eid); //TODO: CHANGE FOR ACTOR VALUES and propper epsilon and change in delete state pls
    
    if (previous_game_speed != c.game_speed)
        state->move.updateStates(IdleState(), AcceleratingState(100), MovingState(), DeceleratingState(EPSILON * c.game_speed)); //TODO: Change too
    
    
    //DOWN
    if (Input::down(Input::Key::S))
        down(c, eid);
    
    
    //GROUNDED
    if (state->jump.is(GroundedState())) {
        if (not checkGroundDown(c, eid))
            state->jump.handle(FallEvent());
    }
    
    
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
        
    
    //JUMP
    if (Input::pressed(Input::Key::Space))
        jump(c, eid);
    
    if (Input::released(Input::Key::Space))
        releaseJump();
    
    if (actor->vel.y > 0 and state->jump.is(JumpingState()))
        state->jump.handle(PeakJumpEvent());
    
    previously_on_air = not (state->jump.is(GroundedState()) or state->jump.is(CrouchingState()));
    falling_tiny_bit = (previously_on_air) ? falling_tiny_bit : false;
        
    state->jump.handle(TimeoutEvent(c.game_speed));
    tried_jumping = state->jump.is(FallingButJumpingState()) or state->jump.is(FallingFasterButJumpingState());
    
    
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
    bool on_ground = state->jump.is(GroundedState()) or state->jump.is(CrouchingState());
    
    
    //FRICTION
    {
        int friction = (on_ground) ? actor->friction_ground : actor->friction_air;
        if (state->move.is(DeceleratingState()))
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
    if (tried_jumping and state->jump.is(JumpingState())) {
        actor->vel.y = -jump_impulse;
        falling_tiny_bit = false;
        anim->queue.push_back("jump_end_short");
        anim->queue.push_back("jump_start_short");
    }
    //TODO: Check if jump released for this :)
        
    
    //ANIMATION
    if (state->move.is(IdleState()) or state->move.is(DeceleratingState())) {
        anim->target_key = curr_idle_anim;
    }
    if (state->move.is(MovingState()) or state->move.is(AcceleratingState())) {
        anim->target_key = "walk_1";
        if (state->jump.is(FallingCoyoteState()))
            falling_tiny_bit = checkGroundDown(c, eid, TINY_BIT);
    }
    
    if (state->jump.is(JumpingState()))
        anim->target_key = "jump_up";
    if (not falling_tiny_bit and (state->jump.is(FallingState()) or state->jump.is(FallingCoyoteState()) or state->jump.is(FallingButJumpingState())))
        anim->target_key = "jump_down";
    if (not falling_tiny_bit and previously_on_air and on_ground) //TODO: Platform check
        anim->queue.push_back("jump_end");
    
    return true;
}

void Controller::Player::move(Config &c, bool right) {
    actor->vel.x += actor->acc_ground * c.physics_delta * (right ? 1 : -1); //TODO: Add acc air
    
    tex->is_reversed = not right;
    
    state->move.handle(MoveEvent(right ? 1 : -1, actor->vel.x));
}

void Controller::Player::jump(Config &c, EntityID eid) {
    if (state->jump.is(JumpingState()))
        return;
    
    state->jump.handle(JumpEvent());
    
    if (state->jump.is(JumpingState())) {
        actor->vel.y = -jump_impulse;
        falling_tiny_bit = false;
        anim->queue.push_back("jump_start");
        
        actor->extra_vel.x += getMovingPlatformVelocity(c, eid).x * (1.0f + 0.5f * abs(actor->vel.x / actor->max_move_speed));
        state->move.handle(MoveEvent(sign(actor->extra_vel.x), actor->extra_vel.x));
    }
}

void Controller::Player::releaseJump() {
    if (not state->jump.is(JumpingState()))
        return;
    
    state->jump.handle(ReleaseJumpEvent());
    
    if (state->jump.is(FallingState()) and -actor->vel.y > min_jump_impulse)
        actor->vel.y = -min_jump_impulse;
}

void Controller::Player::down(Config &c, EntityID eid) {
    collider->transform += Vec2::j;
    System::Collider::CollisionInfo collisions = System::Collider::checkCollisions(c, eid);
    collider->transform -= Vec2::j;
    for (System::Collider::CollisionInfoPair collision : collisions) {
        if (collision.second[System::Collider::Layers::Platform]) {
            state->jump.handle(DownEvent(true));
        }
    }
}

void Controller::Player::respawn(Config &c) {
    auto closest_vec = [=](Vec2 v1, Vec2 v2) -> bool {
        return abs(v1.x - *collider->transform.x) < abs(v2.x - *collider->transform.x); };
    
    Vec2 closest_checkpoint = *std::min_element(c.active_scene->checkpoints.cbegin(), c.active_scene->checkpoints.cend(), closest_vec);
    collider->transform = closest_checkpoint;
    
    actor->vel = Vec2f(0,0);
    state->jump.handle(FallEvent());
    
    light_strength = 100;
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

str Controller::Player::getCurrentJumpState() {
    return (state == nullptr) ? "" : CURR_STATE(state->jump);
}

str Controller::Player::getCurrentMoveState() {
    return (state == nullptr) ? "" : CURR_STATE(state->move);
}

void Controller::Player::resetState(Config &c, EntityID eid) {
    delete state;
    state = new PlayerStates(COYOTE_TIMEOUT, GRACE_TIMEOUT, 100, EPSILON); //TODO: Change
    
    c_state = c.active_scene->getComponent<Component::State>(eid);
    if (c_state == nullptr) {
        log::error("The state component in the player is null!");
        return;
    }
    c_state->state = state;
}
