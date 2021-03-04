//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <math.h>

#include "dtypes.h"
#include "log.h"

#include "game.h"
#include "ecs.h"
#include "scene.h"
#include "config.h"

#include "r_pipeline.h"
#include "gui.h"

#include "s_collider.h"
#include "s_tilemap.h"
#include "player_controller.h"

using namespace Verse;

int main(int argc, const char * argv[]) {
    
    Config config = {
        .name = "Proxecto Lume",
        .version = "0.0.9",
        .resolution = Vec2(256, 180),
        .window_size = Vec2(1024, 720),
        .render_scale = 4,
        .use_shaders = true,
        .use_grayscale = false,
        .palette_index = 0,
        .num_palettes = 4,
        .background_color = {0, 0, 0, 255},
        .render_collision_boxes = false
    };
    
    bool running = Game::init(config);
    
    
    Component::registerComponents();
    
    Scene scene;
    
    
    EntityID test = scene.createEntity("Player");
    
    Component::Collider* collider = scene.addComponent<Component::Collider>(test);
    collider->transform = Rect(32, 64, 9, 14);
    
    Component::Texture* texture = scene.addComponent<Component::Texture>(test);
    texture->tex = Graphics::loadTexture("res/graphics/lume_idle.png");
    texture->transform = Rect(32, 64, 11, 14);
    texture->offset = Vec2(-1, 0);
    texture->animation.push_back(Vec2(0,2));
    
    Component::Actor* actor = scene.addComponent<Component::Actor>(test);
    actor->controller = [test, &scene]() -> bool {return Controller::Player::controller(scene, Entity::getIndex(test));};
    actor->max_move_speed = 100;
    actor->max_fall_speed = 250;
    actor->acc_ground = 1000;
    actor->friction_ground = 0.25;
    
    Component::Light* light = scene.addComponent<Component::Light>(test);
    light->pos = texture->transform.size / 2;
    light->radius = 96;
    light->centre = 0.2;
    
    
    /*EntityID test2 = scene.createEntity("Ground");
    
    Component::Collider* collider2 = scene.addComponent<Component::Collider>(test2);
    collider2->transform = Rect(40, 140, 8, 8);
    
    Component::Texture* texture2 = scene.addComponent<Component::Texture>(test2);
    texture2->tex = Graphics::loadTexture("res/graphics/ground2.png");
    texture2->transform = Rect(40, 140, 8, 8);*/
    
    
    EntityID tile = scene.createEntity("Tilemap");
    
    Component::Tilemap* tilemap = scene.addComponent<Component::Tilemap>(tile);
    tilemap->tiles = System::Tilemap::load("res/levels/1.png");
    tilemap->tex = Graphics::loadTexture("res/graphics/ground2.png");
    tilemap->tex_size = Vec2(8, 8);
    tilemap->pos = Vec2(0, 96);
    
    Component::Collider* tile_collider = scene.addComponent<Component::Collider>(tile);
    tile_collider->transform = Rect(tilemap->pos, System::Tilemap::calculateSize(tilemap));
    
    while (running)
        running = Game::update(scene);
    
    
    Game::stop();
    
    return EXIT_SUCCESS;
}
