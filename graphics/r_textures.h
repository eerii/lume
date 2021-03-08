//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "stb_image.h"

#include "dtypes.h"
#include "log.h"
#include "config.h"

#include "c_texture.h"
#include "c_tilemap.h"

namespace Verse::Graphics
{
    void loadTexture(str path, Component::Texture* tex);
    void loadTexture(str path, Component::Tilemap* tex);
    void loadTexture(str path, ui32 &tex_id);
}
