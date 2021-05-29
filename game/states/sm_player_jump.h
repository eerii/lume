//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "state_machine.h"
#include "static_str.h"
#include "ftime.h"

#define MAKE_STRING(TYPE) [[maybe_unused]] static constexpr auto make_string(Types<TYPE>) { return Str{#TYPE}; };

namespace Verse::State::Player
{

    struct JumpEvent {};
    struct ReleaseJumpEvent {};
    struct PeakJumpEvent {};
    struct FallEvent {};
    struct DownEvent {};
    struct ReleaseDownEvent {};
    struct TouchGroundEvent {};
    struct TimeoutEvent {
        float game_speed = 1.0f;
        TimeoutEvent() = default;
        TimeoutEvent(float p_s) : game_speed(p_s) {};
    };

    struct GroundedState;
    struct JumpingState;
    struct FallingState;
    struct FallingCoyoteState;
    struct FallingButJumpingState;
    struct FallingFasterState;
    struct FallingFasterButJumpingState;
    struct CrouchingState;

    struct GroundedState : Do<
        Default<Nothing>,
        On<JumpEvent, To<JumpingState>>,
        On<FallEvent, To<FallingCoyoteState>>,
        On<DownEvent, To<CrouchingState>>
    > {};

    struct JumpingState : Do<
        Default<Nothing>,
        On<PeakJumpEvent, To<FallingState>>,
        On<ReleaseJumpEvent, To<FallingState>>
    > {};

    struct FallingState : Do<
        Default<Nothing>,
        On<JumpEvent, To<FallingButJumpingState>>,
        On<TouchGroundEvent, To<GroundedState>>,
        On<DownEvent, To<FallingFasterState>>
    > {};

    struct FallingCoyoteState : Default<Nothing> {
        using Default::handle;
        
        ui32 grace_time;
        ui32 timer;
        FallingCoyoteState() = default;
        FallingCoyoteState(ui32 p_time) : grace_time(p_time) {};
        
        void onEnter(const FallEvent &e) {
            timer = setTimer(grace_time);
        }
        
        Maybe<To<FallingState>> handle(const TimeoutEvent &e) {
            if (checkTimer(timer, e.game_speed))
                return To<FallingState>();
            return Nothing();
        }
        
        To<JumpingState> handle(const JumpEvent &e) { return To<JumpingState>(); };
        
        To<GroundedState> handle(const TouchGroundEvent &e) { return To<GroundedState>(); };
    };

    struct FallingButJumpingState : Default<Nothing> {
        using Default::handle;
        
        ui32 grace_time;
        ui32 timer;
        FallingButJumpingState() = default;
        FallingButJumpingState(ui32 p_time) : grace_time(p_time) {};
        
        void onEnter(const JumpEvent &e) {
            timer = setTimer(grace_time);
        }
        
        Maybe<To<FallingState>> handle(const TimeoutEvent &e) {
            if (checkTimer(timer, e.game_speed))
                return To<FallingState>();
            return Nothing();
        }
        
        To<JumpingState> handle(const TouchGroundEvent &e) { return To<JumpingState>(); };
        
        To<FallingFasterButJumpingState> handle(const DownEvent &e) { return To<FallingFasterButJumpingState>(); };
    };

    struct FallingFasterState : Do<
        Default<Nothing>,
        On<JumpEvent, To<FallingFasterButJumpingState>>,
        On<TouchGroundEvent, To<GroundedState>>,
        On<ReleaseDownEvent, To<FallingState>>
    > {};

    struct FallingFasterButJumpingState : Default<Nothing> {
        using Default::handle;
        
        ui32 grace_time;
        ui32 timer;
        FallingFasterButJumpingState() = default;
        FallingFasterButJumpingState(ui32 p_time) : grace_time(p_time) {};
        
        void onEnter(const JumpEvent &e) {
            timer = setTimer(grace_time);
        }
        
        Maybe<To<FallingFasterState>> handle(const TimeoutEvent &e) {
            if (checkTimer(timer, e.game_speed))
                return To<FallingFasterState>();
            return Nothing();
        }
        
        To<JumpingState> handle(const TouchGroundEvent &e) { return To<JumpingState>(); };
        
        //TODO: Keep the same timer as before and don't reset it
        To<FallingButJumpingState> handle(const ReleaseDownEvent &e) { return To<FallingButJumpingState>(); };
    };

    struct CrouchingState : Do<
        Default<Nothing>,
        On<JumpEvent, To<JumpingState>>,
        On<FallEvent, To<FallingCoyoteState>>,
        On<ReleaseDownEvent, To<GroundedState>>
    > {};

    MAKE_STRING(JumpEvent)
    MAKE_STRING(ReleaseJumpEvent)
    MAKE_STRING(PeakJumpEvent)
    MAKE_STRING(FallEvent)
    MAKE_STRING(DownEvent)
    MAKE_STRING(ReleaseDownEvent)
    MAKE_STRING(TouchGroundEvent)
    MAKE_STRING(TimeoutEvent)

    MAKE_STRING(GroundedState)
    MAKE_STRING(JumpingState)
    MAKE_STRING(FallingState)
    MAKE_STRING(FallingCoyoteState)
    MAKE_STRING(FallingButJumpingState)
    MAKE_STRING(FallingFasterState)
    MAKE_STRING(FallingFasterButJumpingState)
    MAKE_STRING(CrouchingState)

    using JumpSM = StateMachine<FallingState, GroundedState, JumpingState, FallingCoyoteState, FallingButJumpingState,
                                FallingFasterState, FallingFasterButJumpingState, CrouchingState>;

}

/*
 JumpSM jump{GroundedState(), JumpingState(), FallingState(), FallingCoyoteState(10), FallingButJumpingState(10)};
 
 jump.handle(JumpEvent{});
 log::num(jump.is(GroundedState()));
 log::num(jump.is(JumpingState()));
 log::info(CURR_STATE(jump));
 */
