//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "state_machine.h"
#include "static_str.h"

#define MAKE_STRING(TYPE) [[maybe_unused]] static constexpr auto make_string(Types<TYPE>) { return Str{#TYPE}; };

namespace Verse::State
{
    namespace Door
    {
        struct OpenEvent {};
        struct CloseEvent {};
        struct LockEvent {
            ui32 new_key;
        };
        struct UnlockEvent {
            ui32 key;
        };

        struct OpenState;
        struct CloseState;
        struct LockedState;

        struct OpenState : Do<Default<Nothing>,
                                      On<CloseEvent, To<CloseState>>> {};

        struct CloseState : Do<Default<Nothing>,
                               On<LockEvent, To<LockedState>>,
                               On<OpenEvent, To<OpenState>>> {};

        struct LockedState : Default<Nothing> {
            using Default::handle;
            
            ui32 key;
            LockedState(ui32 p_key) : key(p_key) {};
            
            void onEnter(const LockEvent &e) {
                key = e.new_key;
            }
            
            Maybe<To<CloseState>> handle(const UnlockEvent& e) {
                if (e.key == key) {
                    log::info("unlocking...");
                    return To<CloseState>();
                }
                else {
                    return Nothing();
                }
            }
        };
    
        MAKE_STRING(OpenEvent)
        MAKE_STRING(CloseEvent)
        MAKE_STRING(LockEvent)
        MAKE_STRING(UnlockEvent)

        MAKE_STRING(OpenState)
        MAKE_STRING(CloseState)
        MAKE_STRING(LockedState)
    }

    using Door_SM = StateMachine<Door::CloseState, Door::OpenState, Door::LockedState>;
}

/*Door_SM door{Door::CloseState{}, Door::OpenState{}, Door::LockedState{0}};

door.handle(Door::LockEvent{1234});
door.handle(Door::UnlockEvent{2});
door.handle(Door::UnlockEvent{1234});

Types stateTypes = Door_SM::getStateTypes();
Types eventTypes = Types<Door::OpenEvent, Door::CloseEvent, Door::LockEvent, Door::UnlockEvent>{};
log::debug_func(stateTypes * eventTypes);
log::debug_func((stateTypes * eventTypes) | MapAndSum(ResolveAction{}));

std::cout << generatePrettyTransitionTable(stateTypes, eventTypes).c_str() << std::endl;*/
