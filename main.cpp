//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include <SDL2/SDL.h>

#include "log.h"
#include "config.h"
#include "game.h"
#include "serialization.h"

#include "r_textures.h"

#include "reflection.h"

#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#endif

using namespace Verse;

namespace {
    Config config;
}

struct Point {
    Serialize(Point, x, y)
    int x;
    int y;
};

struct Circle {
    Serialize(Circle, point, radius)
    Point point;
    float radius;
};

struct TestRefl {
    Serialize(TestRefl, circle, hello, vec, map)
    Circle circle;
    str hello;
    std::vector<ui16> vec;
    std::map<str, float> map;
    //Vec2 v; Doesn't work yet
    //Rect2 r;
};

#ifdef __EMSCRIPTEN__
void game_loop_emscripten() {
    Game::update(config);
}
#endif

int main(int argc, const char * argv[]) {
    
    config = {
        .name = "Proxecto Lume",
        .version = {0, 2, 3},
        
        .resolution = Vec2(256, 180),
        .window_size = Vec2(1024, 720),
        .render_scale = 4,
        .use_vsync = true,
        
        .timestep = 10.0,
        .game_speed = 1.0f,
        
        .use_grayscale = false,
        .use_light = true,
        .palette_index = 1,
        .num_palettes = 9,
        .background_color = {0.0, 0.0, 0.0, 1.0},
        
        .enable_lookahead = true,
        .enable_smooth_panning = true,
        .use_subpixel_cam = true,
        
        .gravity = 800,
        .gravity_dir = Vec2f(0, 1),
        
        .player_loses_light = false,
    };
    
    log::debug("Starting main");
    
    bool running = Game::init(config);
    Component::registerComponents();
    
    Scene* scene = new Scene();
    
    //TODO: DELETE
    //running = false;
    
    TestRefl test{};
    test.vec = {1, 2, 3, 4, 5};
    test.map["hola"] = 3.5;
    test.map["adios"] = 6.9;
    //log::reflection(test);
    
    /*Reflection::forEach(a, [&](auto &&x, ui8 level, const char* name){
        //Check for integers
        if constexpr (std::is_integral_v<std::decay_t<decltype(x)>>)
            log::num(x);
        
        if constexpr (Reflection::is_reflectable<decltype(x)>)
            log::info(x.type_name);
        else {
            std::cout << name << " : " << x << "\n";
        }
    });*/
    
    //TODO: Custom save for members
    //TODO: Fix member placement in YAML
    
    //TODO: Change casts "(float)" to static casts when possible
    //TODO: Change for loops from int to size_t
    //TODO: Check if chrono is better than sdl time
    
#ifdef USE_VULKAN
    Serialization::loadScene("test_vulkan", scene, config);
    config.active_scene = scene;
    for (EntityID e : SceneView<Component::Camera>(*scene))
        config.active_camera = scene->getComponent<Component::Camera>(e);
#else
    Serialization::loadScene("A01_inicio", scene, config); //Always scene before player, if not camera no bounds
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
    
    srand((ui32)time(NULL));
    config.palette_index = (int)(rand() % 5 + 1);
#endif
    
    //Component::Dialogue* dialogue = scene->addComponent<Component::Dialogue>(player);
    //System::Dialogue::parse(config, dialogue, "dialogue/test");
    
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
