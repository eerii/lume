//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "config.h"
#include "imgui.h"

namespace Verse::Gui
{
    void init(Config &c);
    void update(float delta, Config &c);
    void prerender(Scene &scene, Config &c, SDL_Window* window);
    void render();
}
