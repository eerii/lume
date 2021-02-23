//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include <math.h>

#include "s_tilemap.h"

using namespace Verse;

void System::Tilemap::render(Scene &scene, SDL_Renderer* renderer) {
    for (EntityID e : SceneView<Component::Tilemap>(scene)) {
        Component::Tilemap* tmap = scene.getComponent<Component::Tilemap>(e);
        
        SDL_Rect src;
        src.x = 0;
        src.y = 0;
        src.w = tmap->tex_size.x;
        src.h = tmap->tex_size.y;
        
        SDL_Rect dst;
        dst.x = tmap->pos.x * RENDER_SCALE;
        dst.y = tmap->pos.y * RENDER_SCALE;
        dst.w = tmap->tex_size.x * RENDER_SCALE;
        dst.h = tmap->tex_size.y * RENDER_SCALE;
        
        for (int i = 0; i < tmap->tiles.size(); i++) {
            for (int j = 0; j < tmap->tiles[i].size(); j++) {
                if (tmap->tiles[i][j] != 0) {
                    src.x = (tmap->tiles[i][j] - 1) * tmap->tex_size.x;
                    SDL_RenderCopy(renderer, tmap->tex, &src, &dst);
                }
                
                dst.x += tmap->tex_size.x * RENDER_SCALE;
            }
            dst.x = tmap->pos.x * RENDER_SCALE;
            dst.y += tmap->tex_size.y * RENDER_SCALE;
        }
    }
}

Vec2 System::Tilemap::calculateSize(Component::Tilemap* tilemap) {
    Vec2 size;
    
    size.x = tilemap->tiles[0].size() * tilemap->tex_size.x;
    size.y = tilemap->tiles.size() * tilemap->tex_size.y;
    
    return size;
}
