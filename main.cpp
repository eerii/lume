//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include <SDL2/SDL.h>

#include "log.h"
#include "config.h"
#include "game.h"
#include "serialization.h"

#include "r_textures.h"

using namespace Verse;

int main(int argc, const char * argv[]) {
    
    Config config = {
        .name = "Proxecto Lume",
        .version = "0.1.6",
        
        .resolution = Vec2(256, 180),
        .window_size = Vec2(1024, 720),
        .render_scale = 4,
        .enable_gui = true,
        .use_vsync = true,
        
        .game_speed = 1.0f,
        
        .use_grayscale = false,
        .use_light = true,
        .palette_index = 0,
        .num_palettes = 4,
        .background_color = {0.0, 0.0, 0.0, 1.0},
        
        .gravity = 800,
        .gravity_dir = Vec2f(0, 1)
    };
    
    Serialization::initYAML();
    bool running = Game::init(config);
    Component::registerComponents();
    
    
    Scene scene;
    
    Serialization::loadScene("test_scene", scene, config); //Always scene before player, if not camera no bounds
    EntityID player = Serialization::loadPlayer(scene, config);
    config.available_scenes.push_back(&scene);
    
    Scene scene2;
    Serialization::loadScene("test_scene_2", scene2, config);
    config.available_scenes.push_back(&scene2);
    
    EntityID test_transition = scene.createEntity("scene_transition");
    Component::SceneTransition* t = scene.addComponent<Component::SceneTransition>(test_transition);
    t->to_pos = Vec2(32, 64);
    t->to_scene = &scene2;
    Component::Collider* c = scene.addComponent<Component::Collider>(test_transition);
    c->transform = Rect2(150, 80, 30, 30);
    c->layer = Component::ColliderLayers::EVENT;
    Component::Texture* tex = scene.addComponent<Component::Texture>(test_transition);
    Graphics::Texture::loadTexture("res/graphics/palette_multi.png", tex);
    tex->transform = Rect2(150, 80, 30, 30);
    tex->offset.push_back(Vec2(0,0));
    tex->layer.push_back(0);
    
    
    config.active_camera = scene.getComponent<Component::Camera>(player);
    if (config.active_camera == nullptr)
        log::error("Failed to get the active camera!");
    
    config.active_scene = &scene;
    
    
    while (running)
        running = Game::update(config);
    
    
    Game::stop();
    
    return EXIT_SUCCESS;
}
