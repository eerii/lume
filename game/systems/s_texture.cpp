//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_texture.h"
#include "r_renderer.h"

using namespace Verse;

namespace {
    int animation_speed = Graphics::getRefreshRate() / 3; //3 FPS
    int frame_count = 0;
    str curr_key = "";
}

void System::Texture::render(Scene &scene, Config &c) {
    if (frame_count < animation_speed)
        frame_count++;
    
    for (EntityID e : SceneView<Component::Texture>(scene)) {
        Component::Texture* tex = scene.getComponent<Component::Texture>(e);
        
        Rect2 dst = Rect2((tex->transform.pos() + tex->offset), tex->transform.size());
        
        Rect2 src;
        
        Component::Animation* anim = scene.getComponent<Component::Animation>(e);
        if (anim != nullptr) {
            if (curr_key == "")
                curr_key = anim->curr_key;
            if (frame_count >= animation_speed or frame_count == -1) {
                frame_count = -1;
                if (anim->curr_frame < anim->frames[anim->curr_key].size() - 1) {
                    anim->curr_frame++;
                } else {
                    anim->curr_frame = 0;
                }
            }
            if (curr_key != anim->curr_key) {
                curr_key = anim->curr_key;
                anim->curr_frame = 0;
                frame_count = -1;
            }
            src = Rect2(Vec2((tex->transform.w * anim->frames[curr_key][anim->curr_frame]), 0), tex->transform.size());
            Graphics::Renderer::renderTexture(tex->tex_id, src, dst, anim->size, c, tex->is_reversed);
        } else {
            src = Rect2(Vec2(), tex->transform.size());
            Graphics::Renderer::renderTexture(tex->tex_id, src, dst, 1, c, tex->is_reversed);
        }
    }
}
