//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include "log.h"

#include "ecs.h"
#include "scene.h"
#include "r_pipeline.h"
#include "r_textures.h"

namespace Verse::System::Texture
{
    void render(Scene &scene, SDL_Renderer* renderer, Config &c);
}
