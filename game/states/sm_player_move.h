//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "state_machine.h"
#include "static_str.h"

#define MAKE_STRING(TYPE) [[maybe_unused]] static constexpr auto make_string(Types<TYPE>) { return Str{#TYPE}; };

namespace Verse::State::Player
{

    struct MoveEvent {
        ui16 acceleration;
        ui16 speed;
    };
    struct StopMovingEvent {
        ui16 deceleration;
        ui16 speed;
    };

    struct IdleState;
    struct AcceleratingState;
    struct MovingState;
    struct DeceleratingState;

    struct IdleState : Do<
        Default<Nothing>,
        On<MoveEvent, To<AcceleratingState>>
    > {};

    struct MovingState : Do<
        Default<Nothing>,
        On<StopMovingEvent, To<DeceleratingState>>
    > {};

    struct AcceleratingState : Default<Nothing> {
        using Default::handle;
        
        ui16 acceleration;
        ui16 max_speed;
        AcceleratingState(ui16 p_max) : max_speed(p_max) {};
        
        void onEnter(const MoveEvent &e) {
            acceleration = e.acceleration;
        }
        
        Maybe<To<MovingState>> handle(const MoveEvent &e) {
            if (e.speed >= max_speed)
                return To<MovingState>();
            return Nothing();
        }
        
        To<DeceleratingState> handle(const StopMovingEvent &e) { return To<DeceleratingState>(); };
    };

    struct DeceleratingState : Default<Nothing> {
        using Default::handle;
        
        ui16 deceleration;
        ui16 min_speed;
        DeceleratingState(ui16 p_min) : min_speed(p_min) {};
        
        void onEnter(const StopMovingEvent &e) {
            deceleration = e.deceleration;
        }
        
        Maybe<To<IdleState>> handle(const StopMovingEvent &e) {
            if (e.speed <= min_speed)
                return To<IdleState>();
            return Nothing();
        }
        
        To<AcceleratingState> handle(const MoveEvent &e) { return To<AcceleratingState>(); };
    };

    MAKE_STRING(MoveEvent)
    MAKE_STRING(StopMovingEvent)

    MAKE_STRING(IdleState)
    MAKE_STRING(AcceleratingState)
    MAKE_STRING(MovingState)
    MAKE_STRING(DeceleratingState)

    using MoveSM = StateMachine<IdleState, AcceleratingState, MovingState, DeceleratingState>;

}
