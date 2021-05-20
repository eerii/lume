//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "state_machine.h"
#include "static_str.h"
#include "time.h"

#define MAKE_STRING(TYPE) [[maybe_unused]] static constexpr auto make_string(Types<TYPE>) { return Str{#TYPE}; };

namespace Verse::State::Player
{

    struct JumpEvent {};
    struct ReleaseJumpEvent {};
    struct PeakJumpEvent {};
    struct FallEvent {};
    struct FastFallEvent {};
    struct TouchGroundEvent {};
    struct TimeoutEvent {
        ui64 time;
    };

    struct GroundedState;
    struct JumpingState;
    struct FallingState;
    struct FallingCoyoteState;
    struct FallingButJumpingState;

    struct GroundedState : Do<
        Default<Nothing>,
        On<JumpEvent, To<JumpingState>>,
        On<FallEvent, To<FallingCoyoteState>>
    > {};

    struct JumpingState : Do<
        Default<Nothing>,
        On<PeakJumpEvent, To<FallingState>>,
        On<ReleaseJumpEvent, To<FallingState>>
    > {};

    struct FallingState : Do<
        Default<Nothing>,
        On<JumpEvent, To<FallingButJumpingState>>,
        On<TouchGroundEvent, To<GroundedState>>
    > {};

    struct FallingCoyoteState : Default<Nothing> {
        using Default::handle;
        
        ui64 grace_time;
        ui64 enter_time;
        FallingCoyoteState(ui64 p_time) : grace_time(p_time) {};
        
        void onEnter(const FallEvent &e) {
            enter_time = time();
        }
        
        Maybe<To<FallingState>> handle(const TimeoutEvent& e) {
            if (e.time > enter_time + grace_time) {
                return To<FallingState>();
            }
            else {
                return Nothing();
            }
        }
        
        To<JumpingState> handle(const JumpEvent& e) { return To<JumpingState>(); };
        
        To<GroundedState> handle(const TouchGroundEvent& e) { return To<GroundedState>(); };
    };

    struct FallingButJumpingState : Default<Nothing> {
        using Default::handle;
        
        ui64 grace_time;
        ui64 enter_time;
        FallingButJumpingState(ui64 p_time) : grace_time(p_time) {};
        
        void onEnter(const JumpEvent &e) {
            enter_time = time();
        }
        
        Maybe<To<FallingState>> handle(const TimeoutEvent& e) {
            if (e.time > enter_time + grace_time) {
                return To<FallingState>();
            }
            else {
                return Nothing();
            }
        }
        
        To<JumpingState> handle(const TouchGroundEvent& e) { return To<JumpingState>(); };
    };

    MAKE_STRING(JumpEvent)
    MAKE_STRING(ReleaseJumpEvent)
    MAKE_STRING(PeakJumpEvent)
    MAKE_STRING(FallEvent)
    MAKE_STRING(FastFallEvent)
    MAKE_STRING(TouchGroundEvent)
    MAKE_STRING(TimeoutEvent)

    MAKE_STRING(GroundedState)
    MAKE_STRING(JumpingState)
    MAKE_STRING(FallingState)
    MAKE_STRING(FallingCoyoteState)
    MAKE_STRING(FallingButJumpingState)

    using JumpSM = StateMachine<GroundedState, JumpingState, FallingState, FallingCoyoteState, FallingButJumpingState>;

}

/*
 JumpSM jump{GroundedState(), JumpingState(), FallingState(), FallingCoyoteState(10), FallingButJumpingState(10)};
 
 jump.handle(JumpEvent{});
 log::num(jump.is(GroundedState()));
 log::num(jump.is(JumpingState()));
 log::info(CURR_STATE(jump));
 */
