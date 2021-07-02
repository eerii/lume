//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#include "s_text.h"
#include "r_font.h"
#include "r_renderer.h"

#include "gui.h"
#include "gui_types.h"

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
        
        if (text->text != text->previous_text) {
            Graphics::Font::render(text);
            text->previous_text = text->text;
        }
        
        glm::mat4 model = Graphics::Renderer::matModel2D(text->transform.pos - Vec2(BORDER_WIDTH, BORDER_WIDTH), text->transform.size);
        
        Graphics::Renderer::renderText(c, text->tex_id, model, v, text->layer, text->r, text->g, text->b, text->solid_color);
    }
}

void System::Text::load(EntityID eid, YAML::Node &entity, Scene *s, Config &c) {
    Component::Text* text = s->addComponent<Component::Text>(eid);
    if (not entity["text"]["font"]) {
        log::error("You created a text component for " + s->getName(eid) + " but it has no font");
        s->removeEntity(eid);
        return;
    }
    text->font = new FontInfo();
    Graphics::Font::load(text->font, entity["text"]["font"].as<str>());
    
    if (entity["text"]["transform"]) {
        text->transform = entity["text"]["transform"].as<Rect2>();
        text->bitmap_size = text->transform.size;
    }
    
    text->layer = (entity["text"]["layer"]) ? entity["text"]["layer"].as<int>() : -10;
    text->line_height = (entity["text"]["line_height"]) ? entity["text"]["line_height"].as<int>() : 16;
    
    text->r = 1; text->g = 1; text->b = 1;
    if (entity["text"]["color"] and entity["text"]["color"].IsSequence()) {
        std::vector<float> color = entity["text"]["color"].as<std::vector<float>>();
        if (color.size() >= 3) {
            text->r = color[0];
            text->g = color[1];
            text->b = color[2];
        }
    }
    
    text->solid_color = (entity["text"]["solid_color"]) ? entity["text"]["solid_color"].as<bool>() : true;
    text->text = (entity["text"]["text"]) ? entity["text"]["text"].as<str>() : "no text";
    text->previous_text = text->text;
    
    ui8 bitmap[1] = {0};
    text->tex_id = Graphics::Texture::createTexture(bitmap, 0, 0, false);
    Graphics::Font::render(text);
}

void System::Text::save(Component::Text *text, str path, std::vector<str> &key) {
    key[2] = "text";
    
    key[3] = "font";
    Serialization::appendYAML(path, key, (str)text->font->path, true);
    
    key[3] = "text";
    Serialization::appendYAML(path, key, (str)text->text, true);
    
    key[3] = "transform";
    Serialization::appendYAML(path, key, text->transform, true);
    
    key[3] = "color";
    str color = "[" + std::to_string(text->r) + "," + std::to_string(text->g) + "," + std::to_string(text->b) + "]";
    YAML::Node n = YAML::Load(color);
    Serialization::appendYAML(path, key, n, true);
    
    key[3] = "line_height";
    Serialization::appendYAML(path, key, text->line_height, true);
    
    key[3] = "layer";
    Serialization::appendYAML(path, key, text->line_height, true);
}

void System::Text::gui(Config &c, EntityID eid) {
#ifndef DISABLE_GUI
    Component::Text* text = c.active_scene->getComponent<Component::Text>(eid);
    if (text == nullptr)
        return;
    
    Verse::Gui::draw_vec2(text->transform.pos.x, text->transform.pos.y, "pos", eid);
    ImGui::TableNextRow();
    
    Verse::Gui::draw_vec2(text->transform.size.x, text->transform.size.y, "display size", eid);
    ImGui::TableNextRow();
    
    Verse::Gui::draw_vec2(text->bitmap_size.x, text->bitmap_size.y, "bitmap size", eid, [&text](){ Graphics::Font::render(text); });
    ImGui::TableNextRow();
    
    Verse::Gui::draw_int(text->layer, "layer", eid);
    ImGui::TableNextRow();
    
    Verse::Gui::draw_int(text->line_height, "line_height", eid, [&text](){ Graphics::Font::render(text); });
    ImGui::TableNextRow();
    
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();
    ImGui::Text("text");
    
    ImGui::TableSetColumnIndex(1);
    str text_label = "##text" + std::to_string(eid);
    ImGui::SetNextItemWidth(ImGui::GetColumnWidth());
    if (ImGui::InputText(text_label.c_str(), &text->text))
        Graphics::Font::render(text);
    ImGui::TableNextRow();
    
    Verse::Gui::draw_float(text->r, "r", eid, [&text](){ Graphics::Font::render(text); });
    ImGui::TableNextRow();
    Verse::Gui::draw_float(text->g, "g", eid, [&text](){ Graphics::Font::render(text); });
    ImGui::TableNextRow();
    Verse::Gui::draw_float(text->b, "b", eid, [&text](){ Graphics::Font::render(text); });
#endif
}
