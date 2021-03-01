//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_collider.h"
#include "s_tilemap.h"
#include "renderer.h"

using namespace Verse;

std::vector<EntityID> System::Collider::checkObjectCollisions(EntityID eid, Scene &scene) {
    Component::Collider* collider = scene.getComponent<Component::Collider>(eid);
    SDL_Rect rect = collider->transform.toSDL();
    
    collider->is_colliding = false;
    
    std::vector<EntityID> collisions;
    
    for (EntityID e : SceneView<Component::Collider>(scene)) {
        if(e == eid)
            continue;
        
        Component::Collider* test_collider = scene.getComponent<Component::Collider>(e);
        SDL_Rect test_rect = test_collider->transform.toSDL();
        
        test_collider->is_colliding = false;
        
        if(SDL_HasIntersection(&rect, &test_rect)) {
            collisions.push_back(e);
            
            collider->is_colliding = true;
            test_collider->is_colliding = true;
        }
    }
    
    return collisions;
}

bool System::Collider::checkTilemapCollision(Component::Collider* test_col, Component::Collider* tile_col, Component::Tilemap* tilemap) {
    bool is_colliding_with_tile = false;
    
    Vec2 rel_pos = test_col->transform.pos - tile_col->transform.pos;
    Vec2 upleft = Vec2(floor(rel_pos.x / tilemap->tex_size.x), floor(rel_pos.y / tilemap->tex_size.y));
    Vec2 downright = Vec2(floor((rel_pos.x + test_col->transform.size.x - 1) / tilemap->tex_size.x),
                          floor((rel_pos.y + test_col->transform.size.y - 1) / tilemap->tex_size.y));
    
    for (int i = upleft.x; i <= downright.x; i++) {
        if (i < 0 or i >= tilemap->tiles[0].size())
            continue;
        for (int j = upleft.y; j <= downright.y; j++) {
            if (j < 0 or j >= tilemap->tiles.size() - 1)
                continue;
            if (tilemap->tiles[j][i] != 0)
                is_colliding_with_tile = true;
        }
    }
    
    return is_colliding_with_tile;
}

bool System::Collider::checkCollisions(EntityID eid, Scene &scene) {
    bool is_colliding = false;
    
    Component::Collider* collider = scene.getComponent<Component::Collider>(eid);
    std::vector<EntityID> collisions = System::Collider::checkObjectCollisions(eid, scene);
    
    if(collisions.size() > 0) {
        for (const EntityID &e : collisions) {
            Component::Collider* c_col = scene.getComponent<Component::Collider>(e);
            Component::Tilemap* c_tile = scene.getComponent<Component::Tilemap>(e);
            
            if (c_tile != nullptr) {
                bool isCollidingWithTile = System::Collider::checkTilemapCollision(collider, c_col, c_tile);
                if (isCollidingWithTile)
                    is_colliding = true;
                else
                    collider->is_colliding = false;
            } else {
                is_colliding = true;
            }
        }
    }
    
    return is_colliding;
}


void System::Collider::render(Scene &scene, SDL_Renderer* renderer, Config &c) {
    for (EntityID e : SceneView<Component::Texture>(scene)) {
        Component::Collider* collider = scene.getComponent<Component::Collider>(e);
        SDL_Rect rect = Rect(collider->transform.pos * c.render_scale, collider->transform.size * c.render_scale).toSDL();
        
        if (collider->is_colliding) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        }
        
        SDL_RenderDrawRect(renderer, &rect);
    }
}
