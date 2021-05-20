//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_texture.h"

#include "r_renderer.h"
#include "r_pipeline.h"

using namespace Verse;

namespace {
    float animation_speed = (float)Graphics::getRefreshRate() / 6.0f; //3 FPS
    int frame_count = 0;
    str curr_key = "";

    float v[16] = {
         0.0,  1.0,  0.0,  1.0,
         1.0,  1.0,  1.0,  1.0,
         0.0,  0.0,  0.0,  0.0,
         1.0,  0.0,  1.0,  0.0,
    };
}

void System::Texture::render(Config &c) {
    if (frame_count * c.game_speed < animation_speed)
        frame_count++;
    
    for (EntityID e : SceneView<Component::Texture>(*c.active_scene)) {
        Component::Texture* tex = c.active_scene->getComponent<Component::Texture>(e);
        Component::Animation* anim = c.active_scene->getComponent<Component::Animation>(e);
        
        if (anim != nullptr) {
            if (curr_key == "")
                curr_key = anim->curr_key;
            if (frame_count * c.game_speed >= animation_speed or frame_count == -1) {
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
        }
        
        Vec2 size = Vec2((anim != nullptr) ? anim->size : 1, (int)tex->layer.size());
        
        for (int i = 0; i < size.y; i++) {
            glm::mat4 vertices = glm::transpose(glm::make_mat4x4(v));
            vertices[2] /= (float)size.x;
            vertices[3] = (vertices[3] + (float)i) / (float)size.y;
            
            if (anim != nullptr and
                anim->frames.size() > 0 and
                anim->frames.count(curr_key) > 0 and
                anim->frames[curr_key].size() > anim->curr_frame)
                vertices[2] += (float)anim->frames[curr_key][anim->curr_frame] / (float)size.x;
            
            if (tex->is_reversed) {
                vertices[2][0] = vertices[2][1];
                vertices[2][1] = vertices[2][2];
                vertices[2][2] = vertices[2][0];
                vertices[2][3] = vertices[2][1];
            }
            
            vertices = glm::transpose(vertices);
            
            Rect2 dst = Rect2((tex->transform.pos() + tex->offset[i]), tex->transform.size());
            glm::mat4 model = Graphics::Renderer::matModel2D(dst.pos() - Vec2(1,1), dst.size());
            
            Graphics::Renderer::renderTexture(c, tex->tex_id, model, glm::value_ptr(vertices), tex->layer[i]);
        }
    }
}
