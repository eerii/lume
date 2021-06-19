//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_collider.h"

#include "s_tilemap.h"
#include "r_renderer.h"

#include "log.h"
#include "gui.h"
#include "gui_types.h"

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

System::Collider::CollisionInfoPair System::Collider::checkTilemapCollision(Config &c, EntityID eid, Component::Collider* test_col) {
    Component::Collider* tile_col = c.active_scene->getComponent<Component::Collider>(eid);
    Component::Tilemap* tilemap = c.active_scene->getComponent<Component::Tilemap>(eid);
    
    Vec2 rel_pos = test_col->transform.pos - tile_col->transform.pos;
    Vec2 upleft = Vec2(floor(rel_pos.x / tilemap->tex_size.x), floor(rel_pos.y / tilemap->tex_size.y));
    Vec2 downright = Vec2(floor((rel_pos.x + *test_col->transform.w - 1) / tilemap->tex_size.x),
                          floor((rel_pos.y + *test_col->transform.h - 1) / tilemap->tex_size.y));
    
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
    
    return tile_collisions;
}

System::Collider::CollisionInfo System::Collider::checkCollisions(Config &c, EntityID eid) {
    System::Collider::CollisionInfo col_info;
    
    Component::Collider* collider = c.active_scene->getComponent<Component::Collider>(eid);
    std::vector<EntityID> collisions = System::Collider::checkObjectCollisions(c, eid);
    
    if(collisions.size() > 0) {
        bool has_tile_collisions = false;
        bool has_regular_collisions = false;
        
        for (const EntityID &e : collisions) {
            Component::Collider* c_col = c.active_scene->getComponent<Component::Collider>(e);
            
            Component::Tilemap* c_tile = c.active_scene->getComponent<Component::Tilemap>(e);
            if (c_tile == nullptr)
                has_regular_collisions = true;
            if (c_tile != nullptr) {
                col_info[e] = System::Collider::checkTilemapCollision(c, e, collider).second;
                if (col_info[e].any())
                    has_tile_collisions = true;
                continue;
            }
            
            col_info[e].set(c_col->layer);
        }
        
        collider->is_colliding = has_regular_collisions or has_tile_collisions;
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

void System::Collider::load(EntityID eid, YAML::Node &entity, struct Scene *s, Config &c) {
    Component::Collider* collider = s->addComponent<Component::Collider>(eid);
    if (entity["collider"].IsMap()) {
        if (entity["collider"]["transform"])
            collider->transform = entity["collider"]["transform"].as<Rect2>();
        collider->layer = System::Collider::Layers::Ground;
        if (entity["collider"]["layer"]) {
            auto it = std::find(System::Collider::layers_name.begin(),
                                System::Collider::layers_name.end(),
                                entity["collider"]["layer"].as<str>());
            if (it != System::Collider::layers_name.end())
                collider->layer = std::distance(System::Collider::layers_name.begin(), it);
        }
    } else {
        if (entity["collider"].as<str>() == "tile") {
            Component::Tilemap* tilemap = s->getComponent<Component::Tilemap>(eid);
            if (not (entity["tilemap"]["pos"] and tilemap != nullptr)) {
                log::error("You tried to add a tilemap collider for " + s->getName(eid) + ", but it doesn't have a tilemap component");
                s->removeEntity(eid);
                return;
            }
            collider->transform = Rect2(tilemap->pos, System::Tilemap::calculateSize(tilemap));
            collider->layer = System::Collider::Layers::Ground;
        }
    }
}

void System::Collider::save(Component::Collider *col, str path, std::vector<str> &key, bool tile) {
    key[2] = "collider";
    
    if (tile) {
        std::vector<str> tile_key = {key[0], key[1], key[2]};
        Serialization::appendYAML(path, tile_key, (str)"tile", true);
        return;
    }
    
    key[3] = "transform";
    Serialization::appendYAML(path, key, col->transform, true);
    
    key[3] = "layer";
    Serialization::appendYAML(path, key, (str)System::Collider::layers_name[col->layer], true);
}

void System::Collider::gui(Config &c, EntityID eid) {
#ifndef DISABLE_GUI
    Component::Collider* col = c.active_scene->getComponent<Component::Collider>(eid);
    
    Verse::Gui::draw_vec2(*col->transform.x, *col->transform.y, "pos", eid);
    ImGui::TableNextRow();
    Verse::Gui::draw_vec2(*col->transform.w, *col->transform.h, "size", eid);
    ImGui::TableNextRow();
    
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("layer");
    
    ImGui::TableSetColumnIndex(1);
    str layer_label = "##collayer" + std::to_string(eid);
    ImGui::SetNextItemWidth(ImGui::GetColumnWidth());
    ImGui::Combo(layer_label.c_str(), col->layer, System::Collider::layers_name);
#endif
}
