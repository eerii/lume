//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include <math.h>

#include "s_tilemap.h"
#include "r_renderer.h"
#include "stb_image.h"

using namespace Verse;

void System::Tilemap::render(Scene &scene, SDL_Renderer* renderer, Config &c) {
    for (EntityID e : SceneView<Component::Tilemap>(scene)) {
        Component::Tilemap* tmap = scene.getComponent<Component::Tilemap>(e);
        
        Rect src;
        src.pos.x = 0;
        src.pos.y = 0;
        src.size.x = tmap->tex_size.x;
        src.size.y = tmap->tex_size.y;
        
        Rect dst;
        dst.pos.x = tmap->pos.x;
        dst.pos.y = tmap->pos.y;
        dst.size.x = tmap->tex_size.x;
        dst.size.y = tmap->tex_size.y;
        
        for (int i = 0; i < tmap->tiles.size(); i++) {
            for (int j = 0; j < tmap->tiles[i].size(); j++) {
                if (tmap->tiles[i][j] != 0) {
                    src.pos.x = (tmap->tiles[i][j] - 1) * tmap->tex_size.x;
#ifdef USE_OPENGL
                    Graphics::Renderer::GL::renderTexture(tmap->tex_id, src, dst, 0, c, false);
#else
                    SDL_Rect src_sdl = src.toSDL();
                    SDL_Rect dst_sdl = dst.toSDL();
                    SDL_RenderCopy(renderer, tmap->tex, &src_sdl, &dst_sdl);
#endif
                }
                
                dst.pos.x += tmap->tex_size.x;
            }
            dst.pos.x = tmap->pos.x;
            dst.pos.y += tmap->tex_size.y;
        }
    }
}

std::vector<std::vector<ui8>> System::Tilemap::load(str path) {
    std::vector<std::vector<ui8>> tiles = {{}};
    
#ifdef USE_OPENGL
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
#else
    SDL_Surface* map = Graphics::loadSurface(path);
    
    if (map->pixels == nullptr) {
        log::error("The tilemap surface has no pixels");
        return tiles;
    }
    
    for (int j = 0; j < map->h; j++) {
        tiles.push_back({});
        for (int i = 0; i < map->w; i++) {
            ui8 *color = (ui8*)map->pixels + j * map->pitch + i * 4;
            
            if (color[3] == 0) {
                tiles[j].push_back(0);
                continue;
            }
            
            //HANDLE MULTIPLE TILES HERE
            //ui8 luminance = round((c[0] + c[1] + c[2]) / 3);
            tiles[j].push_back(1);
        }
    }
#endif
    
    return tiles;
}

Vec2 System::Tilemap::calculateSize(Component::Tilemap* tilemap) {
    Vec2 size;
    
    size.x = tilemap->tiles[0].size() * tilemap->tex_size.x;
    size.y = tilemap->tiles.size() * tilemap->tex_size.y;
    
    return size;
}
