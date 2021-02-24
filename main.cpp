//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <math.h>

#include "dtypes.h"
#include "log.h"

#include "platform.h"
#include "renderer.h"
#include "gui.h"
#include "config.h"

#include "game.h"
#include "ecs.h"
#include "scene.h"

#include "s_collider.h"
#include "s_tilemap.h"
#include "player.h"

using namespace Verse;

int main(int argc, const char * argv[]) {
    
    Config config = {
        .name = "lume",
        .version = "0.0.6",
        .use_shaders = false,
        .render_collision_boxes = false
    };
    
    bool running = Game::init(config);
    
    
    Component::registerComponents();
    
    Scene scene;
    
    
    EntityID test = scene.createEntity("Player");
    
    Component::Collider* collider = scene.addComponent<Component::Collider>(test);
    collider->transform = Rect(40, 100, 15, 17);
    
    Component::Texture* texture = scene.addComponent<Component::Texture>(test);
    texture->tex = Graphics::loadTexture("res/graphics/lume.png");
    texture->transform = Rect(40, 100, 16, 18);
    texture->offset = Vec2(0, -1);
    
    Component::Actor* actor = scene.addComponent<Component::Actor>(test);
    actor->controller = [test, &scene]() -> bool {return Controller::Player::controller(scene, Entity::getIndex(test));};
    actor->max_move_speed = 100;
    actor->max_fall_speed = 250;
    actor->acc_ground = 1000;
    actor->friction_ground = 0.25;
    actor->jump_impulse = 300;
    actor->jump_grace = 150;
    actor->jump_coyote = 100;
    
    EntityID test2 = scene.createEntity("Ground");
    
    Component::Collider* collider2 = scene.addComponent<Component::Collider>(test2);
    collider2->transform = Rect(40, 140, 16, 16);
    
    Component::Texture* texture2 = scene.addComponent<Component::Texture>(test2);
    texture2->tex = Graphics::loadTexture("res/graphics/ground.png");
    texture2->transform = Rect(40, 140, 16, 16);
    
    
    EntityID tile = scene.createEntity("Tilemap");
    
    Component::Tilemap* tilemap = scene.addComponent<Component::Tilemap>(tile);
    tilemap->tiles = {{1, 0, 0, 0, 0, 0, 0, 1, 0}, {1, 0, 1, 0, 0, 0, 0, 1, 0}, {1, 1, 1, 1, 0, 1, 1, 1, 1}};
    tilemap->tex = Graphics::loadTexture("res/graphics/ground.png");
    tilemap->tex_size = Vec2(16, 16);
    tilemap->pos = Vec2(100, 116);
    
    Component::Collider* tile_collider = scene.addComponent<Component::Collider>(tile);
    tile_collider->transform = Rect(tilemap->pos, System::Tilemap::calculateSize(tilemap));
    
    while (running)
        running = Game::update(scene);
    
    
    Game::stop();
    
    return EXIT_SUCCESS;
}
