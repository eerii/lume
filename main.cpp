//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include <SDL2/SDL.h>

#include "log.h"
#include "config.h"
#include "game.h"
#include "serialization.h"

#include "r_textures.h"

#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#endif

#include "r_font.h"

using namespace Verse;

namespace {
    Config config;
}

#ifdef __EMSCRIPTEN__
void game_loop_emscripten() {
    Game::update(config);
}
#endif

int main(int argc, const char * argv[]) {
    
    config = {
        .name = "Proxecto Lume",
        .version = "0.1.13",
        
        .resolution = Vec2(256, 180),
        .window_size = Vec2(1024, 720),
        .render_scale = 4,
        .enable_gui = true,
        .use_vsync = true,
        .use_subpixel_cam = true,
        
        .game_speed = 1.0f,
        .timestep_modifier = 1.0f,
        
        .use_grayscale = false,
        .use_light = true,
        .palette_index = 0,
        .num_palettes = 4,
        .background_color = {0.0, 0.0, 0.0, 1.0},
        
        .gravity = 800,
        .gravity_dir = Vec2f(0, 1),
        
        .player_loses_light = false,
    };
    
    log::debug("Starting main");
    
    bool running = Game::init(config);
    Component::registerComponents();
    
    
    Scene* scene = new Scene();
    
    Serialization::loadScene("test_scene", scene, config); //Always scene before player, if not camera no bounds
    EntityID player = Serialization::loadPlayer(scene, config);
    
    for (EntityID e : SceneView<Component::SceneTransition>(*scene)) {
        Component::SceneTransition* t = scene->getComponent<Component::SceneTransition>(e);
        t->to_scene = new Scene();
        Serialization::loadScene(t->scene_name, t->to_scene, config);
    }
    
    config.active_camera = scene->getComponent<Component::Camera>(player);
    if (config.active_camera == nullptr)
        log::error("Failed to get the active camera!");
    
    config.active_scene = scene;
    
    EntityID test_text = scene->createEntity("test_text");
    Component::Text* text = scene->addComponent<Component::Text>(test_text);
    text->font = new FontInfo();
    Graphics::Font::load(text->font, "res/fonts/test.ttf");
    Graphics::Font::render(text, "hello there", Vec2(100, 40), 20);
    text->transform = Rect2(64, 80, 100, 40);
    
#ifdef __EMSCRIPTEN__
    while (true)
        emscripten_set_main_loop(game_loop_emscripten, 0, 1);
#else
    while (running)
        running = Game::update(config);
#endif
    
    
    Game::stop();
    
    return EXIT_SUCCESS;
}
