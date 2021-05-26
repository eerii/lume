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
        int sign_acc;
        int speed;
        MoveEvent(int p_a, int p_s) : sign_acc(p_a), speed(p_s) {};
    };
    struct StopMovingEvent {
        int speed;
        StopMovingEvent(int p_s) : speed(p_s) {};
    };

    struct IdleState;
    struct AcceleratingState;
    struct MovingState;
    struct DeceleratingState;

    struct IdleState : Do<
        Default<Nothing>,
        On<MoveEvent, To<AcceleratingState>>
    > {};

    struct MovingState : Default<Nothing> {
        using Default::handle;
        
        Maybe<To<AcceleratingState>> handle(const MoveEvent &e) {
            if (e.sign_acc != sign(e.speed))
                return To<AcceleratingState>();
            return Nothing();
        }
        
        To<DeceleratingState> handle(const StopMovingEvent &e) { return To<DeceleratingState>(); };
    };

    struct AcceleratingState : Default<Nothing> {
        using Default::handle;
        
        ui16 max_speed;
        AcceleratingState() = default;
        AcceleratingState(ui16 p_max) : max_speed(p_max) {};
        
        Maybe<To<MovingState>> handle(const MoveEvent &e) {
            if (abs(e.speed) >= max_speed and e.sign_acc == sign(e.speed))
                return To<MovingState>();
            return Nothing();
        }
        
        To<DeceleratingState> handle(const StopMovingEvent &e) { return To<DeceleratingState>(); };
    };

    struct DeceleratingState : Default<Nothing> {
        using Default::handle;
        
        ui16 min_speed;
        DeceleratingState() = default;
        DeceleratingState(ui16 p_min) : min_speed(p_min) {};
        
        Maybe<To<IdleState>> handle(const StopMovingEvent &e) {
            if (abs(e.speed) <= min_speed)
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
