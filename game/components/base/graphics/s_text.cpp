//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_text.h"
#include "r_renderer.h"

using namespace Verse;

namespace {
    float v[16] = {
         0.0,  1.0,  0.0,  1.0,
         1.0,  1.0,  1.0,  1.0,
         0.0,  0.0,  0.0,  0.0,
         1.0,  0.0,  1.0,  0.0,
    };
}

void System::Text::render(Config &c) {
    for (EntityID e : SceneView<Component::Text>(*c.active_scene)) {
        Component::Text* text = c.active_scene->getComponent<Component::Text>(e);
        
        glm::mat4 model = Graphics::Renderer::matModel2D(text->transform.pos - Vec2(BORDER_WIDTH, BORDER_WIDTH), text->transform.size);
        
        Graphics::Renderer::renderText(c, text->tex_id, model, v, -10);
    }
}
