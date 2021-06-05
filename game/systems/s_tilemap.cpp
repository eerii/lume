//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_tilemap.h"

#include <array>

#include "log.h"
#include "input.h"

#include "r_pipeline.h"
#include "r_renderer.h"
#include "r_window.h"
#include "stb_image.h"

using namespace Verse;

std::vector<std::vector<ui8>> System::Tilemap::load(str path) {
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
        src.x = 0;
        src.y = 0;
        src.w = tmap->tex_size.x;
        src.h = tmap->tex_size.y;
        
        Rect2 dst;
        dst.x = tmap->pos.x;
        dst.y = tmap->pos.y;
        dst.w = tmap->tex_size.x;
        dst.h = tmap->tex_size.y;
        
        int k = 0;
        for (int i = 0; i < tmap->tiles.size(); i++) {
            for (int j = 0; j < tmap->tiles[i].size(); j++) {
                if (tmap->tiles[i][j] == l) {
                    src.x = (tmap->tiles[i][j] - 1) * tmap->tex_size.x;
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
                dst.x += tmap->tex_size.x;
            }
            dst.x = tmap->pos.x;
            dst.y += tmap->tex_size.y;
        }
    }
}

void System::Tilemap::render(Config &c) {
    for (EntityID e : SceneView<Component::Tilemap>(*c.active_scene)) {
        Component::Tilemap* tmap = c.active_scene->getComponent<Component::Tilemap>(e);
        
        if (c.tilemap_editor and tmap != c.current_tilemap_edit)
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
    size.y = (int)(tmap->tiles.size() * tmap->tex_size.y);
    
    return size;
}

void System::Tilemap::renderEditor(Config &c) {
    Vec2 t_size = calculateSize(c.current_tilemap_edit) - c.current_tilemap_edit->tex_size;
    Vec2 t_pos = c.current_tilemap_edit->pos;
    
    Vec2 m_pos = Graphics::Window::windowToScene(c, Input::mouse());
    
    Vec2 rel_pos = m_pos - t_pos;
    
    if (rel_pos.x >= 0 and rel_pos.y >= 0 and rel_pos.x < t_size.x and rel_pos.y < t_size.y) {
        Vec2 curr_tile = Vec2(floor((float)rel_pos.x / (float)c.current_tilemap_edit->tex_size.x),
                              floor((float)rel_pos.y / (float)c.current_tilemap_edit->tex_size.y));
        Vec2 curr_pos = t_pos + Vec2(curr_tile.x * c.current_tilemap_edit->tex_size.x,
                                     curr_tile.y * c.current_tilemap_edit->tex_size.y);
        
        Graphics::Renderer::renderDebugCollider(c, Rect2(curr_pos, c.current_tilemap_edit->tex_size), false);
        
        if (Input::down((ui8)Input::MouseButton::Left)) {
            c.current_tilemap_edit->tiles[curr_tile.y][curr_tile.x] = 0;
            createVertices(c, c.current_tilemap_edit);
        }
        
        if (Input::down((ui8)Input::MouseButton::Right)) {
            c.current_tilemap_edit->tiles[curr_tile.y][curr_tile.x] = 255;
            createVertices(c, c.current_tilemap_edit);
        }
    }
}
