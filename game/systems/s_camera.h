//project verse, 2017-2021
//by jose pazos perez
//all rights reserved uwu

#pragma once

#include "dtypes.h"
#include "log.h"
#include "config.h"
#include "ecs.h"
#include "scene.h"
#include "c_camera.h"

namespace Verse::System::Camera
{
    void init(Component::Camera* camera, Vec2f pos, Vec2f size);
    void setActive(Component::Camera* camera);
    void setActive(Component::Camera* camera, Rect2 cam_bounds);
    void setActive(Component::Camera* camera, Scene &scene);
    void update(Config &c, Scene& scene);
    void move(Config &c, Vec2f pos, int input);
    void updatePoints(Config &c, Vec2f &pos);
    void checkBounds(Config &c);
}
