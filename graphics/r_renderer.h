//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include "log.h"
#include "config.h"

namespace Verse::Graphics::Renderer::GL
{
    void create(Config &c, SDL_Window* window, ui8 &pid);

    ui32 create_texture(Tex* tex, int w, int h);
    void render_texture(ui32 &tex_id, Rect &src, Rect &dst, ui16 frames);

    void clear();
    void present(SDL_Window* window, ui8 &pid);
    void destroy();
}
