//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include <math.h>

#include "s_tilemap.h"

using namespace Verse;

void System::Tilemap::render(Scene &scene, SDL_Renderer* renderer, Config &c) {
    for (EntityID e : SceneView<Component::Tilemap>(scene)) {
        Component::Tilemap* tmap = scene.getComponent<Component::Tilemap>(e);
        
        SDL_Rect src;
        src.x = 0;
        src.y = 0;
        src.w = tmap->tex_size.x;
        src.h = tmap->tex_size.y;
        
        SDL_Rect dst;
        dst.x = tmap->pos.x * c.render_scale;
        dst.y = tmap->pos.y * c.render_scale;
        dst.w = tmap->tex_size.x * c.render_scale;
        dst.h = tmap->tex_size.y * c.render_scale;
        
        for (int i = 0; i < tmap->tiles.size(); i++) {
            for (int j = 0; j < tmap->tiles[i].size(); j++) {
                if (tmap->tiles[i][j] != 0) {
                    src.x = (tmap->tiles[i][j] - 1) * tmap->tex_size.x;
                    SDL_RenderCopy(renderer, tmap->tex, &src, &dst);
                }
                
                dst.x += tmap->tex_size.x * c.render_scale;
            }
            dst.x = tmap->pos.x * c.render_scale;
            dst.y += tmap->tex_size.y * c.render_scale;
        }
    }
}

std::vector<std::vector<ui8>> System::Tilemap::load(str path) {
    std::vector<std::vector<ui8>> tiles = {{}};
    
#ifdef USE_OPENGL
    //TODO: Load Tilemap
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
