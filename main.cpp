//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include <SDL2/SDL.h>

#include "dtypes.h"
#include "log.h"
#include "config.h"
#include "serialization.h"

#include "game.h"
#include "scene.h"
#include "system_list.h"

#include "state_machine.h"

using namespace Verse;

namespace {

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

struct OpenState : State::Do<State::Default<State::Nothing>,
                             State::On<CloseEvent, State::To<CloseState>>> {};

struct CloseState : State::Do<State::Default<State::Nothing>,
                              State::On<LockEvent, State::To<LockedState>>,
                              State::On<OpenEvent, State::To<OpenState>>> {};

struct LockedState : State::Default<State::Nothing> {
    using Default::handle;
    
    ui32 key;
    LockedState(ui32 p_key) : key(p_key) {};
    
    void onEnter(const LockEvent &e) {
        key = e.new_key;
    }
    
    State::ThisOrNothing<State::To<CloseState>> handle(const UnlockEvent& e) {
        if (e.key == key) {
            log::info("unlocking...");
            return State::To<CloseState>();
        }
        else {
            return State::Nothing();
        }
    }
};

using Door = State::StateMachine<CloseState, OpenState, LockedState>;

}

int main(int argc, const char * argv[]) {
    
    Config config = {
        .name = "Proxecto Lume",
        .version = "0.1.5",
        .resolution = Vec2(256, 180),
        .window_size = Vec2(1024, 720),
        .render_scale = 4,
        .game_speed = 1.0f,
        .enable_gui = true,
        .use_vsync = true,
        .use_grayscale = false,
        .use_light = true,
        .palette_index = 0,
        .num_palettes = 4,
        .background_color = {0.0, 0.0, 0.0, 1.0},
        .render_collision_boxes = false
    };
    
    
    /*Door door{CloseState{}, OpenState{}, LockedState{0}};
    
    door.handle(LockEvent{1234});
    door.handle(UnlockEvent{2});
    door.handle(UnlockEvent{1234});*/
    
    
    Serialization::initYAML();
    bool running = Game::init(config);
    Component::registerComponents();
    
    
    Scene scene;
    
    Serialization::loadScene("test_scene", scene, config); //Always scene before player, if not camera no bounds
    EntityID player = Serialization::loadPlayer(scene, config);
    config.active_camera = scene.getComponent<Component::Camera>(player);
    if (config.active_camera == nullptr)
        log::error("Failed to get the active camera!");
    
    
    while (running)
        running = Game::update(config, scene);
    
    
    Game::stop();
    
    return EXIT_SUCCESS;
}
