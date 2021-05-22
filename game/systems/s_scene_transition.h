//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "config.h"

namespace Verse::System::SceneTransition
{
    void handle(Config &c, Component::SceneTransition* transition);
    void handle(Config &c, Scene* new_scene, Vec2 new_pos);
}
