//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include "log.h"
#include "config.h"

namespace Verse::Graphics::Renderer::GL
{
    void create(Config &c, SDL_Window* window);

    ui32 createTexture(Tex* tex, int w, int h);
    void renderTexture(ui32 &tex_id, Rect &src, Rect &dst, ui16 frames, Config &c, bool flip);

    void clear(Config &c);
    void render(Config &c);
    void present(SDL_Window* window);
    void destroy();
}
