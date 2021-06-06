//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_collider.h"

#include "s_tilemap.h"
#include "r_renderer.h"

#include "log.h"

using namespace Verse;

namespace {

}

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

System::Collider::CollisionInfoPair System::Collider::checkTilemapCollision(Config &c, EntityID eid, Component::Collider* test_col) {
    Component::Collider* tile_col = c.active_scene->getComponent<Component::Collider>(eid);
    Component::Tilemap* tilemap = c.active_scene->getComponent<Component::Tilemap>(eid);
    
    Vec2 rel_pos = test_col->transform.pos() - tile_col->transform.pos();
    Vec2 upleft = Vec2(floor(rel_pos.x / tilemap->tex_size.x), floor(rel_pos.y / tilemap->tex_size.y));
    Vec2 downright = Vec2(floor((rel_pos.x + test_col->transform.w - 1) / tilemap->tex_size.x),
                          floor((rel_pos.y + test_col->transform.h - 1) / tilemap->tex_size.y));
    
    System::Collider::CollisionInfoPair tile_collisions;
    tile_collisions.first = eid;
    
    for (int i = upleft.x; i <= downright.x; i++) {
        if (i < 0 or i >= tilemap->tiles[0].size())
            continue;
        for (int j = upleft.y; j <= downright.y; j++) {
            if (j < 0 or j >= tilemap->tiles.size() - 1)
                continue;
            if (tilemap->tiles[j][i] < 255)
                tile_collisions.second.set(System::Collider::Layers::Ground);
        }
    }
    
    if (tile_collisions.second.none())
        test_col->is_colliding = false;
    
    return tile_collisions;
}

System::Collider::CollisionInfo System::Collider::checkCollisions(Config &c, EntityID eid) {
    System::Collider::CollisionInfo col_info;
    
    Component::Collider* collider = c.active_scene->getComponent<Component::Collider>(eid);
    std::vector<EntityID> collisions = System::Collider::checkObjectCollisions(c, eid);
    
    if(collisions.size() > 0) {
        for (const EntityID &e : collisions) {
            Component::Collider* c_col = c.active_scene->getComponent<Component::Collider>(e);
            
            Component::Tilemap* c_tile = c.active_scene->getComponent<Component::Tilemap>(e);
            if (c_tile != nullptr) {
                col_info[e] = System::Collider::checkTilemapCollision(c, e, collider).second;
                continue;
            }
            
            col_info[e].set(c_col->layer);
        }
    }
    
    return col_info;
}


void System::Collider::render(Config &c) {
    for (EntityID e : SceneView<Component::Collider>(*c.active_scene)) {
        Component::Collider* collider = c.active_scene->getComponent<Component::Collider>(e);
        
        if (c.active_scene->getComponent<Component::Tilemap>(e) != nullptr)
            continue;
        
        Graphics::Renderer::renderDebugCollider(c, collider->transform, collider->is_colliding);
    }
}

/*if (tiles.w == 0)
    tiles = Rect2(i * tilemap->tex_size.x, j * tilemap->tex_size.y, tilemap->tex_size.x, tilemap->tex_size.y);

if (i * tilemap->tex_size.x < tiles.x) {
    tiles.w += tiles.x - i * tilemap->tex_size.x;
    tiles.x = i * tilemap->tex_size.x;
}
if (j * tilemap->tex_size.y < tiles.y) {
    tiles.h += tiles.y - j * tilemap->tex_size.y;
    tiles.y = j * tilemap->tex_size.y;
}
if (i * tilemap->tex_size.x >= tiles.x + tilemap->tex_size.x) {
    tiles.w += i * tilemap->tex_size.x - tiles.x;
}
if (j * tilemap->tex_size.y >= tiles.y + tilemap->tex_size.y) {
    tiles.h += j * tilemap->tex_size.y - tiles.y;
}*/
