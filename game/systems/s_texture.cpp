//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_texture.h"

#include "ftime.h"
#include "r_renderer.h"
#include "r_pipeline.h"

#include "log.h"

using namespace Verse;

namespace {
    ui32 timer = 0;
    str curr_key = "";
    ui16 queue_left = 0;

    float v[16] = {
         0.0,  1.0,  0.0,  1.0,
         1.0,  1.0,  1.0,  1.0,
         0.0,  0.0,  0.0,  0.0,
         1.0,  0.0,  1.0,  0.0,
    };
}

void System::Texture::render(Config &c) {
    for (EntityID e : SceneView<Component::Texture>(*c.active_scene)) {
        Component::Texture* tex = c.active_scene->getComponent<Component::Texture>(e);
        Component::Animation* anim = c.active_scene->getComponent<Component::Animation>(e);
        
        if (anim != nullptr) {
            if (timer == 0)
                timer = setTimer(anim->frames[anim->curr_key].ms[anim->curr_frame]);
            
            if (curr_key == "")
                curr_key = anim->curr_key;
            
            if (checkTimer(timer, c.game_speed) or (queue_left == 0 and anim->queue.size() > 0)) {
                queue_left = (ui16)anim->queue.size();
                
                anim->curr_frame = (anim->curr_frame < anim->frames[anim->curr_key].index.size() - 1) ? anim->curr_frame + 1 : 0;
                curr_key = anim->curr_key;
                
                if (anim->queue.size() > 0) {
                    curr_key = anim->queue[0];
                    anim->queue.erase(anim->queue.begin());
                    anim->curr_key = curr_key;
                    anim->curr_frame = 0;
                }
                
                timer = setTimer(anim->frames[curr_key].ms[anim->curr_frame]);
            }
            
            if (queue_left == 0 and curr_key != anim->curr_key) {
                curr_key = anim->curr_key;
                anim->curr_frame = 0;
                anim->change_now = false;
                stopTimer(timer);
                timer = setTimer(anim->frames[curr_key].ms[anim->curr_frame]);
            }
            
            //log::info("%s %s %d %d %d", curr_key.c_str(), anim->curr_key.c_str(), time(), anim->queue.size(), queue_left);
        }
        
        Vec2 size = Vec2((anim != nullptr) ? anim->size : 1, (int)tex->layer.size());
        
        for (int i = 0; i < size.y; i++) {
            glm::mat4 vertices = glm::transpose(glm::make_mat4x4(v));
            vertices[2] /= (float)size.x;
            vertices[3] = (vertices[3] + (float)i) / (float)size.y;
            
            if (anim != nullptr and
                anim->frames.size() > 0 and
                anim->frames.count(curr_key) > 0 and
                anim->frames[curr_key].index.size() > anim->curr_frame)
                vertices[2] += (float)anim->frames[curr_key].index[anim->curr_frame] / (float)size.x;
            
            if (tex->is_reversed) {
                vertices[2][0] = vertices[2][1];
                vertices[2][1] = vertices[2][2];
                vertices[2][2] = vertices[2][0];
                vertices[2][3] = vertices[2][1];
            }
            
            vertices = glm::transpose(vertices);
            
            Rect2 dst = Rect2((tex->transform.pos() + tex->offset[i]), tex->transform.size());
            glm::mat4 model = Graphics::Renderer::matModel2D(dst.pos() - Vec2(BORDER_WIDTH, BORDER_WIDTH), dst.size());
            
            Graphics::Renderer::renderTexture(c, tex->tex_id, model, glm::value_ptr(vertices), tex->layer[i]);
        }
    }
}

str Verse::System::Texture::getCurrKey() {
    return curr_key;
}

void Verse::System::Texture::clean() {
    curr_key = "";
    stopTimer(timer);
    timer = 0;
    queue_left = 0;
}
