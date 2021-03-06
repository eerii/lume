//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_texture.h"
#include "r_renderer.h"

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
        
        Rect src;
        
        if (tex->animation.size() > 0) {
            if (frame_count >= animation_speed or frame_count == -1) {
                frame_count = -1;
                if (tex->current_animation < tex->animation[0].y and tex->current_animation >= tex->animation[0].x) {
                    tex->current_animation++;
                } else {
                    tex->current_animation = tex->animation[0].x;
                }
            }
            src = Rect(Vec2((tex->transform.size.x * tex->current_animation), 0), tex->transform.size);
        } else {
            src = Rect(Vec2(), tex->transform.size);
        }
        
        Rect dst = Rect((tex->transform.pos + tex->offset), tex->transform.size);
        
#ifdef USE_OPENGL
        Graphics::Renderer::GL::renderTexture(tex->tex_id, src, dst, (ui16)(tex->animation.size() + 1), c);
#else
        
        SDL_Rect sdl_src = src.toSDL();
        SDL_Rect sdl_dst = dst.toSDL();
        
        SDL_RendererFlip flip = tex->is_reversed ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        
        SDL_RenderCopyEx(renderer, tex->tex, &sdl_src, &sdl_dst, 0, NULL, flip);
        
#endif
    }
}
