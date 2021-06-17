//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_tilemap.h"

#include <array>

#include "log.h"
#include "input.h"

#include "r_pipeline.h"
#include "r_renderer.h"
#include "r_textures.h"
#include "r_window.h"
#include "stb_image.h"

using namespace Verse;

void System::Tilemap::init(Config &c) {
    for (EntityID e : SceneView<Component::Tilemap>(*c.active_scene)) {
        createVertices(c, c.active_scene->getComponent<Component::Tilemap>(e));
    }
}

void System::Tilemap::createVertices(Config &c, Component::Tilemap* tmap) {
    tmap->vert = {};
    for (int l = 0; l < tmap->tex_id.size(); l++) {
        tmap->vert.push_back({});
        
        Rect2 src;
        src.pos = Vec2(0,0);
        src.size = tmap->tex_size;
        
        Rect2 dst;
        dst.pos = tmap->pos;
        dst.size = tmap->tex_size;
        
        int k = 0;
        for (int i = 0; i < tmap->tiles.size(); i++) {
            for (int j = 0; j < tmap->tiles[i].size(); j++) {                if (tmap->tiles[i][j] == l) {
                    src.pos.x = (tmap->tiles[i][j] - 1) * tmap->tex_size.x;
                    std::array<float, 24> v = {
                        0.0,  1.0,  0.0,  1.0,
                        1.0,  1.0,  1.0,  1.0,
                        0.0,  0.0,  0.0,  0.0,
                        1.0,  1.0,  1.0,  1.0,
                        0.0,  0.0,  0.0,  0.0,
                        1.0,  0.0,  1.0,  0.0,
                    };
                    tmap->vert[l].push_back(v);
                    Graphics::Renderer::prepareTilemap(c, dst, tmap->vert[l][k]);
                    k++;
                }
                dst.pos.x += tmap->tex_size.x;
            }
            dst.pos.x = tmap->pos.x;
            dst.pos.y += tmap->tex_size.y;
        }
    }
}

void System::Tilemap::render(Config &c) {
    for (EntityID e : SceneView<Component::Tilemap>(*c.active_scene)) {
        Component::Tilemap* tmap = c.active_scene->getComponent<Component::Tilemap>(e);
        
        if (c.tme_active and tmap != c.tme_curr_tmap)
            continue;
        
        int i = 0;
        for (ui32 t : tmap->tex_id) {
            Graphics::Renderer::renderTilemap(c, t, reinterpret_cast<float*>(tmap->vert[i].data()), (int)tmap->vert[i].size(), tmap->layer);
            i++;
        }
    }
}

Vec2 System::Tilemap::calculateSize(Component::Tilemap* tmap) {
    Vec2 size;
    
    size.x = (int)(tmap->tiles[0].size() * tmap->tex_size.x);
    size.y = (int)((tmap->tiles.size() - 1) * tmap->tex_size.y);
    
    return size;
}

std::vector<std::vector<ui8>> System::Tilemap::loadFromImage(str path) {
    std::vector<std::vector<ui8>> tiles = {{}};

    int w, h, ch;
    ui8* map = stbi_load(path.c_str(), &w, &h, &ch, STBI_grey);
    int pitch = w * sizeof(ui8);
    
    if (map == nullptr) {
        log::error("The tilemap has no pixels");
        return tiles;
    }
    
    for (int j = 0; j < h; j++) {
        tiles.push_back({});
        for (int i = 0; i < w; i++) {
            ui8 *color = (ui8*)map + j * pitch + i;
            tiles[j].push_back(*color);
        }
    }
    
    return tiles;
}

void System::Tilemap::load(EntityID eid, YAML::Node &entity, Scene *s, Config &c) {
    Component::Tilemap* tilemap = s->addComponent<Component::Tilemap>(eid);
    if (not entity["tilemap"]["tiles"]) {
        log::error("You created a tilemap component for " + s->getName(eid) + " but it has no tile attribute");
        s->removeEntity(eid);
        return;
    }
    tilemap->tile_res = entity["tilemap"]["tiles"].as<str>();
    tilemap->tiles = System::Tilemap::loadFromImage(tilemap->tile_res);
    if (not entity["tilemap"]["res"]) {
        log::error("You created a tilemap component for " + s->getName(eid) + " but it has no res for the texture");
        s->removeEntity(eid);
        return;
    }
    tilemap->res = (entity["tilemap"]["res"].IsScalar()) ?
                    std::vector<str>({entity["tilemap"]["res"].as<str>()}) :
                    entity["tilemap"]["res"].as<std::vector<str>>();
    Graphics::Texture::loadTexture(tilemap->res, tilemap);
    if (entity["tilemap"]["pos"])
        tilemap->pos = entity["tilemap"]["pos"].as<Vec2>();
    if (entity["tilemap"]["tex_size"])
        tilemap->tex_size = entity["tilemap"]["tex_size"].as<Vec2>();
    tilemap->layer = (entity["tilemap"]["layer"]) ? entity["tilemap"]["layer"].as<int>() : 0;
}

void System::Tilemap::renderEditor(Config &c) {
    Vec2 t_size = calculateSize(c.tme_curr_tmap);
    Vec2 t_pos = c.tme_curr_tmap->pos;
    Graphics::Renderer::renderDebugCollider(c, Rect2(t_pos.x, t_pos.y, t_size.x, t_size.y+1), false);
    
    Vec2 m_pos = Graphics::Window::windowToScene(c, Input::mouse());
    Vec2 rel_pos = m_pos - t_pos;
    
    if (rel_pos.x >= 0 and rel_pos.y >= 0 and rel_pos.x < t_size.x and rel_pos.y < t_size.y) {
        Vec2 curr_tile = Vec2(floor((float)rel_pos.x / (float)c.tme_curr_tmap->tex_size.x),
                              floor((float)rel_pos.y / (float)c.tme_curr_tmap->tex_size.y));
        Vec2 curr_pos = t_pos + Vec2(curr_tile.x * c.tme_curr_tmap->tex_size.x,
                                     curr_tile.y * c.tme_curr_tmap->tex_size.y);
        
        Graphics::Renderer::renderDebugCollider(c, Rect2(curr_pos, c.tme_curr_tmap->tex_size), false);
        
        if (Input::down((ui8)Input::MouseButton::Left)) {
            c.tme_curr_tmap->tiles[curr_tile.y][curr_tile.x] = 0;
            createVertices(c, c.tme_curr_tmap);
        }
        
        if (Input::down((ui8)Input::MouseButton::Right)) {
            c.tme_curr_tmap->tiles[curr_tile.y][curr_tile.x] = 255;
            createVertices(c, c.tme_curr_tmap);
        }
    }
}
