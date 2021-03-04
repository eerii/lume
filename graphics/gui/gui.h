//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include <SDL2/SDL.h>

#include "dtypes.h"
#include "log.h"

#include "imgui.h"
#include "imgui_sdl.h"

#include "scene.h"
#include "config.h"

namespace Verse::Gui
{
    void update(float delta, Config &c);
    void prerender(Scene &scene, Config &c, ui16 &fps);
    void render();
}