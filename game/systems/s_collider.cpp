//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_collider.h"

#include "s_tilemap.h"
#include "s_scene_transition.h"
#include "r_renderer.h"

using namespace Verse;

std::vector<EntityID> System::Collider::checkObjectCollisions(Config &c, EntityID eid) {
    Component::Collider* collider = c.active_scene->getComponent<Component::Collider>(eid);
    if (collider == nullptr)
        return {};
    collider->is_colliding = false;
    
    Component::Actor* actor = c.active_scene->getComponent<Component::Actor>(eid);
    if (actor == nullptr)
        return {};
    
    SDL_Rect rect = collider->transform.toSDL();
    std::vector<EntityID> collisions;
    
    for (EntityID e : SceneView<Component::Collider>(*c.active_scene)) {
        if(e == eid)
            continue;
        
        Component::Collider* test_collider = c.active_scene->getComponent<Component::Collider>(e);
        test_collider->is_colliding = false;
        
        if (not actor->collision_mask[test_collider->layer])
            continue;
        
        SDL_Rect test_rect = test_collider->transform.toSDL();
        
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
    
    Vec2 rel_pos = test_col->transform.pos() - tile_col->transform.pos();
    Vec2 upleft = Vec2(floor(rel_pos.x / tilemap->tex_size.x), floor(rel_pos.y / tilemap->tex_size.y));
    Vec2 downright = Vec2(floor((rel_pos.x + test_col->transform.w - 1) / tilemap->tex_size.x),
                          floor((rel_pos.y + test_col->transform.h - 1) / tilemap->tex_size.y));
    
    for (int i = upleft.x; i <= downright.x; i++) {
        if (i < 0 or i >= tilemap->tiles[0].size())
            continue;
        for (int j = upleft.y; j <= downright.y; j++) {
            if (j < 0 or j >= tilemap->tiles.size() - 1)
                continue;
            if (tilemap->tiles[j][i] < 255)
                is_colliding_with_tile = true;
        }
    }
    
    return is_colliding_with_tile;
}

std::bitset<MAX_COLLISION_LAYERS> System::Collider::checkCollisions(Config &c, EntityID eid) {
    std::bitset<MAX_COLLISION_LAYERS> collision_layers;
    
    Component::Collider* collider = c.active_scene->getComponent<Component::Collider>(eid);
    std::vector<EntityID> collisions = System::Collider::checkObjectCollisions(c, eid);
    
    if(collisions.size() > 0) {
        for (const EntityID &e : collisions) {
            Component::Collider* c_col = c.active_scene->getComponent<Component::Collider>(e);
            
            //Tilemap
            Component::Tilemap* c_tile = c.active_scene->getComponent<Component::Tilemap>(e);
            if (c_tile != nullptr) {
                bool is_colliding_with_tile = System::Collider::checkTilemapCollision(collider, c_col, c_tile);
                if (is_colliding_with_tile)
                    collision_layers.set(Component::ColliderLayers::GROUND);
                else
                    collider->is_colliding = false;
                continue;
            }
            
            //Scene Transition
            Component::SceneTransition* c_transition = c.active_scene->getComponent<Component::SceneTransition>(e);
            if (c_transition != nullptr) {
                System::SceneTransition::handle(c, c_transition);
                collision_layers.set(Component::ColliderLayers::SCENE);
                continue;
            }
            
            collision_layers.set(c_col->layer);
        }
    }
    
    return collision_layers;
}


void System::Collider::render(Config &c) {
    /*for (EntityID e : SceneView<Component::Texture>(scene)) {
        Component::Collider* collider = scene.getComponent<Component::Collider>(e);
        Rect rect = Rect(collider->transform.pos * c.render_scale, collider->transform.size * c.render_scale);
        
        if (collider->is_colliding) {
            
        } else {
            
        }
        
        //Draw rect
    }*/
}
