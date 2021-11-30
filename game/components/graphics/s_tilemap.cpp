//project fresa, 2017-2022
//by jose pazos perez
//all rights reserved uwu

#include "s_tilemap.h"

#include <glm/ext.hpp>

#include "log.h"
#include "input.h"

#include "r_pipeline.h"
#include "r_renderer.h"
#include "r_window.h"
#include "stb_image.h"

#include "gui.h"
#include "gui_types.h"
#ifndef DISABLE_GUI
#include "gui_tilemap_editor.h"
#endif

using namespace Fresa;

namespace {
#ifndef USE_VULKAN
    Component::Camera* prev_cam; //TODO: CHANGE
    Component::Camera* tile_cam;
#endif
}

void System::Tilemap::init(Config &c) {
    for (EntityID e : SceneView<Component::Tilemap>(*c.active_scene)) {
        createVertices(c, c.active_scene->getComponent<Component::Tilemap>(e));
    }
}

void System::Tilemap::createVertices(Config &c, Component::Tilemap* tmap) {
    tmap->vert = {};
    for (int l = 0; l < tmap->tex_data.size(); l++) {
        tmap->vert.push_back({});
        
        Rect2 src(0, 0, tmap->tex_size.x, tmap->tex_size.y);
        Rect2 dst(tmap->pos, tmap->tex_size);
        
        for (int i = 0; i < tmap->tiles.size(); i++) {
            for (int j = 0; j < tmap->tiles[i].size(); j++) {
                if (tmap->tiles[i][j] == l) {
                    src.x = (tmap->tiles[i][j] - 1) * tmap->tex_size.x;
                    float v[24] = {
                        0.0,  1.0,  0.0,  1.0,
                        1.0,  1.0,  1.0,  1.0,
                        0.0,  0.0,  0.0,  0.0,
                        1.0,  1.0,  1.0,  1.0,
                        0.0,  0.0,  0.0,  0.0,
                        1.0,  0.0,  1.0,  0.0,
                    };
                    
                    for (int i = 0; i < 6; i++) {
                        v[4*i + 0] = (i % 2 == 1) ? dst.w : 0.0;
                        v[4*i + 0] += dst.x;
                        v[4*i + 1] = (i < 2 or i == 3) ? dst.h : 0.0;
                        v[4*i + 1] += dst.y;
                    }
                
                    for (int i = 0; i < 24; i++)
                        tmap->vert[l].push_back(v[i]);
                }
                dst.x += tmap->tex_size.x;
            }
            dst.x = tmap->pos.x;
            dst.y += tmap->tex_size.y;
        }
        
        //TODO: Redo
        /*tmap->tex_data[l].vertices = tmap->vert[l];
        tmap->tex_data[l].model = glm::translate(glm::mat4(1.0f), glm::vec3(-BORDER_WIDTH, -BORDER_WIDTH, 0.0f));
        tmap->tex_data[l].layer = tmap->layer;*/
    }
}

void System::Tilemap::render(Config &c) {
    for (EntityID e : SceneView<Component::Tilemap>(*c.active_scene)) {
        Component::Tilemap* tmap = c.active_scene->getComponent<Component::Tilemap>(e);
        
        if (c.tme_active and tmap != c.tme_curr_tmap)
            continue;
        
        if (tmap->res.size() == 0 or tmap->tex_data.size() == 0)
            continue;
        
        int i = 0;
        for (Graphics::TextureData t : tmap->tex_data) {
            Graphics::Renderer::renderTilemap(c, t);
            i++;
        }
    }
}

Vec2<> System::Tilemap::calculateSize(Component::Tilemap* tmap) {
    Vec2 size;
    
    size.x = (int)(tmap->tiles[0].size() * tmap->tex_size.x);
    size.y = (int)((tmap->tiles.size() - 1) * tmap->tex_size.y);
    
    return size;
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
            c.tme_curr_tmap->tiles[curr_tile.y][curr_tile.x] = c.tme_curr_tile;
            createVertices(c, c.tme_curr_tmap);
        }
        
        if (Input::down((ui8)Input::MouseButton::Right)) {
            c.tme_curr_tmap->tiles[curr_tile.y][curr_tile.x] = 255;
            createVertices(c, c.tme_curr_tmap);
        }
    }
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

void System::Tilemap::saveToImage(str path, std::vector<std::vector<ui8>> &tiles) {
    int h = (int)tiles.size() - 1;
    int w = (int)tiles[0].size();
    
    ui8 data[w * h];
    
    int index = 0;
    
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            data[index++] = tiles[j][i];
        }
    }
    
    //TEXTURE stbi_write_png(path.c_str(), w, h, 1, data, w);
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
    //TEXTURE Graphics::Texture::loadTexture(tilemap->res, tilemap);
    if (entity["tilemap"]["pos"])
        tilemap->pos = entity["tilemap"]["pos"].as<Vec2<>>();
    if (entity["tilemap"]["tex_size"])
        tilemap->tex_size = entity["tilemap"]["tex_size"].as<Vec2<>>();
    tilemap->layer = (entity["tilemap"]["layer"]) ? entity["tilemap"]["layer"].as<int>() : 0;
}

void System::Tilemap::save(Component::Tilemap *tile, str path, std::vector<str> &key) {
    key[2] = "tilemap";
    
    key[3] = "res";
    if (tile->res.size() == 1)
        Serialization::appendYAML(path, key, (str)tile->res[0], true);
    else
        Serialization::appendYAML(path, key, tile->res, true);
    
    key[3] = "tiles";
    Serialization::appendYAML(path, key, (str)tile->tile_res, true);
    
    key[3] = "pos";
    Serialization::appendYAML(path, key, tile->pos, true);
    
    key[3] = "tex_size";
    Serialization::appendYAML(path, key, tile->tex_size, true);
    
    key[3] = "layer";
    Serialization::appendYAML(path, key, tile->layer, true);
}

void System::Tilemap::gui(Config &c, EntityID eid) {
#ifndef DISABLE_GUI
    Component::Tilemap* tile = c.active_scene->getComponent<Component::Tilemap>(eid);
    if (tile == nullptr)
        return;
    
    if (tile_cam == nullptr) {
        for (EntityID e : SceneView<Component::Camera>(*c.active_scene)) {
            if (c.active_scene->getName(e) != "free_camera")
                continue;
            tile_cam = c.active_scene->getComponent<Component::Camera>(e); //TODO: CHANGE THIS AND SPAWN A CAMERA
        }
    }
    
    Gui::tilemapEditor(c, eid);
    
    
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();
    ImGui::Text("level");
    
    ImGui::TableSetColumnIndex(1);
    float line_height = ImGui::GetStyle().FramePadding.y * 2.0f + ImGui::CalcTextSize("X").y;
    ImVec2 button_size = { line_height + 3.0f, line_height };
    str button_label = "L##tile" + std::to_string(eid);
    if (ImGui::Button(button_label.c_str(), button_size)) {
        std::vector<std::vector<ui8>> loaded_tiles = loadFromImage(tile->tile_res);
        if (loaded_tiles.size() > 1) {
            tile->tiles = loaded_tiles;
            System::Tilemap::createVertices(c, tile);
        }
    }
    
    ImGui::SameLine();
    str tileres_label = "##tileres" + std::to_string(eid);
    ImGui::SetNextItemWidth(ImGui::GetColumnWidth());
    ImGui::InputText(tileres_label.c_str(), &tile->tile_res);
    ImGui::TableNextRow();
    
    
    for (int i = 0; i < tile->res.size(); i++) {
        str label = "res " + std::to_string(i);
        ImGui::TableSetColumnIndex(0);
        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s", label.c_str());
        
        ImGui::TableSetColumnIndex(1);
        float line_height = ImGui::GetStyle().FramePadding.y * 2.0f + ImGui::CalcTextSize("X").y;
        ImVec2 button_size = { line_height + 3.0f, line_height };
        
        button_label = "L##res" + std::to_string(i) + std::to_string(eid);
        if (ImGui::Button(button_label.c_str(), button_size)) {
            //TEXTURE Graphics::Texture::loadTexture(tile->res, tile);
            System::Tilemap::createVertices(c, tile);
        }
        
        if (tile->res.size() > 1) {
            ImGui::SameLine();
            button_label = "X##res" + std::to_string(i) + std::to_string(eid);
            if (ImGui::Button(button_label.c_str(), button_size)) {
                tile->res.erase(tile->res.begin() + i);
                //TEXTURE Graphics::Texture::loadTexture(tile->res, tile);
            }
        }
        
        ImGui::SameLine();
        str res_label = "##res" + std::to_string(i) + std::to_string(eid);
        ImGui::SetNextItemWidth(ImGui::GetColumnWidth());
        ImGui::InputText(res_label.c_str(), &tile->res[i]);
        
        ImGui::TableNextRow();
    }
    
    
    if (c.tme_active and c.tme_curr_tmap == tile and tile->res.size() > 1) {
        ImGui::TableSetColumnIndex(0);
        ImGui::AlignTextToFramePadding();
        ImGui::Text("paint res");
        
        ImGui::TableSetColumnIndex(1);
        str i_label = "##paint" + std::to_string(eid);
        ImGui::SetNextItemWidth(ImGui::GetColumnWidth());
        ImGui::DragInt(i_label.c_str(), reinterpret_cast<int*>(&c.tme_curr_tile), 1, 0, (int)tile->res.size() - 1);
        
        ImGui::TableNextRow();
    }
    
    
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();
    
    str add_label = "add res##" + std::to_string(eid);
    if (ImGui::SmallButton(add_label.c_str())) {
        tile->res.push_back(tile->res[0]);
        //TEXTURE Graphics::Texture::loadTexture(tile->res, tile);
        System::Tilemap::createVertices(c, tile);
    }
    
    ImGui::SameLine();
    
    if (c.tme_active and c.tme_curr_tmap == tile) {
        if (ImGui::SmallButton("save")) {
            saveToImage(c.tme_curr_tmap->tile_res, c.tme_curr_tmap->tiles);
            
            c.tme_active = false;
            c.use_light = true;
            c.tme_curr_tmap = nullptr;
            c.tme_curr_id = 0;
            c.tme_curr_tile = 0;
            c.active_camera = prev_cam;
        }
    } else {
        if (ImGui::SmallButton("edit")) {
            c.tme_active = true;
            c.use_light = false;
            c.tme_curr_tmap = tile;
            c.tme_curr_id = eid;
            c.tme_curr_tile = 0;
            prev_cam = c.active_camera;
            tile_cam->pos = prev_cam->pos;
            c.active_camera = tile_cam;
        }
    }
#endif
}
