//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include <SDL2/SDL.h>

#include "log.h"
#include "config.h"
#include "game.h"
#include "serialization.h"

using namespace Verse;

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
