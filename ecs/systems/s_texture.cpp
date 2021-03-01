//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_texture.h"

using namespace Verse;

namespace {
    int animation_speed = Graphics::getRefreshRate() / 3; //3 FPS
    int frame_count = 0;
}

void System::Texture::render(Scene &scene, SDL_Renderer* renderer, Config &c) {
    if (frame_count < animation_speed)
        frame_count++;
    
    for (EntityID e : SceneView<Component::Texture>(scene)) {
        Component::Texture* tex = scene.getComponent<Component::Texture>(e);
        
        Rect frame;
        
        if (tex->animation.size() > 0) {
            if (frame_count >= animation_speed or frame_count == -1) {
                frame_count = -1;
                if (tex->current_animation < tex->animation[0].y and tex->current_animation >= tex->animation[0].x) {
                    tex->current_animation++;
                } else {
                    tex->current_animation = tex->animation[0].x;
                }
            }
            frame = Rect(Vec2((tex->transform.size.x * tex->current_animation), 0), tex->transform.size);
        } else {
            frame = Rect(Vec2(), tex->transform.size);
        }
        
        SDL_Rect src = frame.toSDL();
        SDL_Rect dst = Rect((tex->transform.pos + tex->offset) * c.render_scale, tex->transform.size * c.render_scale).toSDL();
        
        SDL_RenderCopy(renderer, tex->tex, &src, &dst);
    }
}
