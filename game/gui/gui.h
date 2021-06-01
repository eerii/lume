//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#ifdef __EMSCRIPTEN__
#define DISABLE_GUI
#endif

#ifndef DISABLE_GUI

#include "config.h"
#include "imgui.h"

namespace Verse::Gui
{
    struct ActiveWindows {
        static bool entities;
        static bool test;
    };

    void init(Config &c);
    void update(Config &c);
    void prerender(Config &c, SDL_Window* window);
    void render();
}

#endif
