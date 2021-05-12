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

using namespace Verse;

int main(int argc, const char * argv[]) {
    
    Config config = {
        .name = "Proxecto Lume",
        .version = "0.1.4",
        .resolution = Vec2(256, 180),
        .window_size = Vec2(1024, 720),
        .render_scale = 4,
        .enable_gui = true,
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
    Component::Camera* camera = scene.getComponent<Component::Camera>(player);
    System::Camera::setActive(camera);
    
    
    //init(scene, config);
    Game::setActiveScene(&scene);
    
    while (running)
        running = Game::update();
    
    
    Game::stop();
    
    return EXIT_SUCCESS;
}
