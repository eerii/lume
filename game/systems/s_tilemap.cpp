//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include <array>

#include "s_tilemap.h"
#include "r_renderer.h"
#include "stb_image.h"

using namespace Verse;

std::vector<std::vector<ui8>> System::Tilemap::load(str path) {
    std::vector<std::vector<ui8>> tiles = {{}};

    int w, h, ch;
    ui8* map = stbi_load(path.c_str(), &w, &h, &ch, STBI_rgb_alpha);
    int pitch = w * sizeof(ui32); //RGBA pitch
    
    if (map == nullptr) {
        log::error("The tilemap has no pixels");
        return tiles;
    }
    
    for (int j = 0; j < h; j++) {
        tiles.push_back({});
        for (int i = 0; i < w; i++) {
            ui8 *color = (ui8*)map + j * pitch + i * 4;
            
            if (color[3] == 0) {
                tiles[j].push_back(0);
                continue;
            }
            
            //HANDLE MULTIPLE TILES HERE
            //ui8 luminance = round((c[0] + c[1] + c[2]) / 3);
            tiles[j].push_back(1);
        }
    }
    
    return tiles;
}

void System::Tilemap::init(Scene &scene, Config &c) {
    for (EntityID e : SceneView<Component::Tilemap>(scene)) {
        Component::Tilemap* tmap = scene.getComponent<Component::Tilemap>(e);
        createVertices(tmap, c);
    }
}

void System::Tilemap::createVertices(Component::Tilemap *tmap, Config &c) {
    tmap->vert = {};
    
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
            if (tmap->tiles[i][j] != 0) {
                src.x = (tmap->tiles[i][j] - 1) * tmap->tex_size.x;
                std::array<float, 24> v = {
                    0.0,  1.0,  0.0,  1.0,
                    1.0,  1.0,  1.0,  1.0,
                    0.0,  0.0,  0.0,  0.0,
                    1.0,  1.0,  1.0,  1.0,
                    0.0,  0.0,  0.0,  0.0,
                    1.0,  0.0,  1.0,  0.0,
                };
                tmap->vert.push_back(v);
                Graphics::Renderer::prepareTilemap(dst, c, tmap->vert[k]);
                k++;
            }
            dst.x += tmap->tex_size.x;
        }
        dst.x = tmap->pos.x;
        dst.y += tmap->tex_size.y;
    }
}

void System::Tilemap::render(Scene &scene, Config &c) {
    for (EntityID e : SceneView<Component::Tilemap>(scene)) {
        Component::Tilemap* tmap = scene.getComponent<Component::Tilemap>(e);
        
        Graphics::Renderer::renderTilemap(tmap->tex_id, reinterpret_cast<float*>(tmap->vert.data()), (int)tmap->vert.size(), c, tmap->layer);
    }
}

Vec2 System::Tilemap::calculateSize(Component::Tilemap* tmap) {
    Vec2 size;
    
    size.x = (int)(tmap->tiles[0].size() * tmap->tex_size.x);
    size.y = (int)(tmap->tiles.size() * tmap->tex_size.y);
    
    return size;
}
