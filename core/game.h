//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include "log.h"
#include "scene.h"
#include "renderer.h"
#include "config.h"

#define TIMESTEP 10 //Update the simulation every 10ms (0.01s)
#define DELTA (TIMESTEP * 0.001f)

namespace Verse::Game
{
    bool init(Config &c);

    bool update(Scene &scene);
    void time_frame();

    void stop();
}
